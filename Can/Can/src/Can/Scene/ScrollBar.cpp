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

		sceneRegistry.emplace<TransformComponent>(entityID, parameters.Position);
		sceneRegistry.emplace<SpriteRendererComponent>(entityID, parameters.Size, nullptr, parameters.BaseColor);

		auto bar = sceneRegistry.create();
		sceneRegistry.emplace<ChildrenComponent>(entityID, std::vector<entt::entity>{bar});
		sceneRegistry.emplace<ParentComponent>(bar, entityID);
		sceneRegistry.emplace<IgnoreCollisionComponent>(bar);

		float ratio = fixedSize ? sizeRatio : 1.0f / parameters.oversize;
		glm::vec3 size{
			parameters.Size.x * (vertical ? 0.9f : ratio),
			parameters.Size.y * (vertical ? ratio : 0.9f),
			parameters.Size.z
		};
		glm::vec3 offset = (parameters.Size - size) * barPosition;
		sceneRegistry.emplace<TransformComponent>(bar, parameters.Position + offset + glm::vec3{ 0.0f, 0.0f, 0.001f });
		sceneRegistry.emplace<SpriteRendererComponent>(bar, size, nullptr, parameters.BarColor);

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
}