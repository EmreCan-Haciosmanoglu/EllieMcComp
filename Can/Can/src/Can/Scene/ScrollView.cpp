#include "canpch.h"
#include "ScrollView.h"
#include "Can.h"

namespace Can
{
	ScrollView::ScrollView(ScrollViewConstructorParameters& parameters, ScrollBarConstructorParameters _ScrollBarConstructorParameters)
		: sceneRegistry(parameters.Registry)
		, entityID(sceneRegistry.create())
	{
		if (parameters.Parent != entt::null)
			sceneRegistry.emplace<ParentComponent>(entityID, parameters.Parent);

		sceneRegistry.emplace<TransformComponent>(entityID, parameters.Position);
		sceneRegistry.emplace<SpriteRendererComponent>(entityID, parameters.Size, nullptr, parameters.Color, parameters.border, parameters.borderRadius, parameters.BorderColor);

		if (parameters.OnClick)
			sceneRegistry.emplace<OnClickCallbackComponent>(entityID, parameters.OnClick);

		_ScrollBarConstructorParameters.Position = parameters.Position;

		if (parameters.scrollPosition == 0)
		{
			_ScrollBarConstructorParameters.Size.x = parameters.Size.x;
		}
		else if (parameters.scrollPosition == 1)
		{
			_ScrollBarConstructorParameters.Size.y = parameters.Size.y;
			_ScrollBarConstructorParameters.Position.x += parameters.Size.x - _ScrollBarConstructorParameters.Size.x;
		}
		else if (parameters.scrollPosition == 2)
		{
			_ScrollBarConstructorParameters.Size.x = parameters.Size.x;
			_ScrollBarConstructorParameters.Position.y += parameters.Size.y - _ScrollBarConstructorParameters.Size.y;
		}
		else
		{
			_ScrollBarConstructorParameters.Size.y = parameters.Size.y;
		}

		_ScrollBarConstructorParameters.Parent = entt::null;
		scrollbar = new ScrollBar(_ScrollBarConstructorParameters);
	}
	ScrollView::~ScrollView()
	{
		if (sceneRegistry.has<ChildrenComponent>(entityID))
		{
			ChildrenComponent& children = sceneRegistry.get<ChildrenComponent>(entityID);
			for (auto child : children)
				sceneRegistry.remove<ParentComponent>(child);
		}
		if (sceneRegistry.has<ParentComponent>(entityID))
		{
			entt::entity parent = sceneRegistry.get<ParentComponent>(entityID);
			std::vector<entt::entity>& children = sceneRegistry.get<ChildrenComponent>(parent);
			if (children.size() > 1)
				children.erase(std::find(children.begin(), children.end(), entityID));
			else
				sceneRegistry.remove<ChildrenComponent>(parent);
		}

		delete scrollbar;
		sceneRegistry.destroy(entityID);
	}

	void ScrollView::Update()
	{
		const SpriteRendererComponent& vsr = sceneRegistry.get<SpriteRendererComponent>(entityID);
		const TransformComponent& vtransform = sceneRegistry.get<TransformComponent>(entityID);

		if (sceneRegistry.has<ChildrenComponent>(entityID))
		{
			std::vector<entt::entity>& children = sceneRegistry.get<ChildrenComponent>(entityID);
			float totalChildrenWidth = 0.0f;
			float totalChildrenHeight = 0.0f; // If vertical
			float spaceBetween = 0.2f; // needs to be member of this scrollview
			totalChildrenWidth += spaceBetween;
			for (entt::entity child : children)
			{
				if (sceneRegistry.has< IgnoreScrollingComponent>(child))
					continue;
				const SpriteRendererComponent& sr = sceneRegistry.get<SpriteRendererComponent>(child);
				totalChildrenWidth += sr.size.x;
				totalChildrenWidth += spaceBetween;
			}

			entt::entity scrollbarID = scrollbar->entityID;
			if (totalChildrenWidth > vsr.size.x)
			{
				// Add ScrollBar, if not exist, to the selected side "scrollPosition"
				if (std::find(children.begin(), children.end(), scrollbarID) == children.end())
				{
					children.push_back(scrollbarID);
					sceneRegistry.emplace<ParentComponent>(scrollbarID, entityID);
				}
				// Set scrollbar size
				entt::entity bar = sceneRegistry.get<ChildrenComponent>(scrollbarID)[0];
				auto& [bt, bsr] = sceneRegistry.get<TransformComponent, SpriteRendererComponent>(bar);
				scrollbar->sizeRatio = totalChildrenWidth / vsr.size.x;
				bsr.size.x = vsr.size.x / scrollbar->sizeRatio;
				// Then get the scroll bar pos and calculate children positions
				float sizeDiff = totalChildrenWidth - vsr.size.x;

				float leftExtra = sizeDiff * scrollbar->barPosition;
				float rightExtra = sizeDiff - leftExtra;

				glm::vec3 cursor = vtransform.Position;
				cursor.x -= leftExtra;
				cursor.x += spaceBetween;
				cursor.y += spaceBetween;
				cursor.z += 0.01f;

				leftExtra -= spaceBetween;
				rightExtra -= spaceBetween;

				for (int i = 0; i < children.size(); i++)
				{
					entt::entity child = children[i];
					if (sceneRegistry.has< IgnoreScrollingComponent>(child))
						continue;
					SpriteRendererComponent& sr = sceneRegistry.get<SpriteRendererComponent>(child);
					TransformComponent& transform = sceneRegistry.get<TransformComponent>(child);
					sr.trim = {
						0.0f,
						1.0f,
						1.0f,
						0.0f
					};
					if (leftExtra > 0.0f)
					{
						float ratio = (leftExtra > sr.size.x) ? 1.0f : leftExtra / sr.size.x; // may not need if statement
						sr.trim[3] = ratio;
						leftExtra -= sr.size.x;
						leftExtra -= spaceBetween;
					}

					transform.Position = cursor;
					cursor.x += sr.size.x; //horizontal
					cursor.x += spaceBetween;
				}
				for (int i = (int)(children.size()) - 1; i >= 0; i--)
				{
					entt::entity child = children[i];
					if (sceneRegistry.has< IgnoreScrollingComponent>(child))
						continue;
					SpriteRendererComponent& sr = sceneRegistry.get<SpriteRendererComponent>(child);
					TransformComponent& transform = sceneRegistry.get<TransformComponent>(child);

					cursor.x -= sr.size.x; //horizontal
					cursor.x -= spaceBetween;

					if (rightExtra > 0.0f)
					{
						float ratio = (rightExtra > sr.size.x) ? 1.0f : rightExtra / sr.size.x; // may not need if statement
						sr.trim[1] = 1.0f - ratio;
						rightExtra -= sr.size.x;
						rightExtra -= spaceBetween;
					}
				}
			}
			else
			{
				// Remove ScrollBar , if exist, from the view
				auto it = std::find(children.begin(), children.end(), scrollbarID);
				if (it != children.end())
				{
					children.erase(it);
					sceneRegistry.remove<ParentComponent>(scrollbarID);
				}
				// Then snap children to the left
				glm::vec3 cursor = vtransform.Position;
				cursor.x += spaceBetween;
				cursor.y += spaceBetween;
				cursor.z += 0.01f;
				for (entt::entity child : children)
				{
					if (sceneRegistry.has< IgnoreScrollingComponent>(child))
						continue;
					SpriteRendererComponent& sr = sceneRegistry.get<SpriteRendererComponent>(child);
					TransformComponent& transform = sceneRegistry.get<TransformComponent>(child);
					transform.Position = cursor;
					cursor.x += sr.size.x; //horizontal
					cursor.x += spaceBetween;
				}
			}
		}
	}
}