#include "canpch.h"
#include "ScrollBar.h"

namespace Can
{
	ScrollBar::ScrollBar(const ScrollBarConstructorParameters& parameters)
		: sceneRegistry(parameters.Registry)
		, entityID(sceneRegistry.create())
		, vertical(parameters.vertical)
		, fixedSize(parameters.fixedSize)
		, sizeRatio(parameters.sizeRatio)
		, barPosition(parameters.barPosition)
		, OnDragCallback(parameters.OnDrag)
	{
		if (parameters.Parent != entt::null)
			sceneRegistry.emplace<ParentComponent>(entityID, parameters.Parent);

		sceneRegistry.emplace<IgnoreScrollingComponent>(entityID);
		sceneRegistry.emplace<TransformComponent>(entityID, parameters.Position);
		sceneRegistry.emplace<SpriteRendererComponent>(entityID, parameters.Size, nullptr, parameters.BaseColor, parameters.baseBorder, parameters.baseBorderRadius, parameters.baseBorderColor);

		auto bar = sceneRegistry.create();
		sceneRegistry.emplace<ChildrenComponent>(entityID, std::vector<entt::entity>{bar});
		sceneRegistry.emplace<ParentComponent>(bar, entityID);
		sceneRegistry.emplace<IgnoreCollisionComponent>(bar);

		float ratio = fixedSize ? sizeRatio : 1.0f / parameters.oversize;
		glm::vec2 size{
			parameters.Size.x * (vertical ? 0.9f : ratio),
			parameters.Size.y * (vertical ? ratio : 0.9f)
		};
		glm::vec2 offset = (parameters.Size - size) * barPosition;
		sceneRegistry.emplace<TransformComponent>(bar, parameters.Position + glm::vec3(offset, 0.0f) + glm::vec3{ 0.0f, 0.0f, 0.001f });
		sceneRegistry.emplace<SpriteRendererComponent>(bar, size, nullptr, parameters.BarColor, parameters.barBorder, parameters.barBorderRadius, parameters.barBorderColor);

		if (parameters.OnClick)
			sceneRegistry.emplace<OnClickCallbackComponent>(entityID, parameters.OnClick);
	}
	ScrollBar::~ScrollBar()
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
		sceneRegistry.destroy(entityID);
	}
	bool ScrollBar::Update(const glm::vec2& mousePos)
	{
		entt::entity barID = sceneRegistry.get<ChildrenComponent>(entityID)[0];

		const TransformComponent& sbTransform = sceneRegistry.get<TransformComponent>(entityID);
		const SpriteRendererComponent& sbsr = sceneRegistry.get<SpriteRendererComponent>(entityID);


		TransformComponent& bTransform = sceneRegistry.get<TransformComponent>(barID);
		SpriteRendererComponent& bsr = sceneRegistry.get<SpriteRendererComponent>(barID);

		glm::vec2 ts = (mousePos - (glm::vec2{ sbTransform.Position.x, sbTransform.Position.y } + bsr.size * 0.5f)) / (sbsr.size - bsr.size);
		ts = { std::clamp(ts.x, 0.0f, 1.0f), std::clamp(ts.y, 0.0f, 1.0f) };

		glm::vec2 diffTs = (ts - barPosition) * (sbsr.size - bsr.size);
		glm::vec3 movement = {
			vertical ? 0.0f : diffTs.x,
			vertical ? diffTs.y : 0.0f,
			0.0f
		};
		bTransform.Position += movement;

		if (barPosition == (vertical ? ts.y : ts.x))
			return false;

		barPosition = vertical ? ts.y : ts.x;
		return true;
	}
}