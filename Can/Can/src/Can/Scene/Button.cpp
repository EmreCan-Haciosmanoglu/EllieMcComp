#include "canpch.h"
#include "Button.h"

namespace Can
{
	Button::Button(const ButtonConstructorParameters& parameters)
		: sceneRegistry(parameters.Registry)
		, entityID(sceneRegistry.create())
	{
		if (parameters.Parent != entt::null)
			sceneRegistry.emplace<ParentComponent>(entityID, parameters.Parent);

		sceneRegistry.emplace<TransformComponent>(entityID, parameters.Position);
		sceneRegistry.emplace<SpriteRendererComponent>(entityID, parameters.Size, parameters.Texture, parameters.Color, parameters.border, parameters.borderRadius, parameters.BorderColor);

		if (parameters.OnClick)
			sceneRegistry.emplace<OnClickCallbackComponent>(entityID, parameters.OnClick);
	}
	Button::~Button()
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