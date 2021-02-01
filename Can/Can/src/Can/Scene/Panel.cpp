#include "canpch.h"
#include "Panel.h"
#include <glm/gtx/matrix_decompose.hpp>

namespace Can
{
	Panel::Panel(const PanelConstructorParameters& parameters)
		: sceneRegistry(parameters.Registry)
		, entityID(sceneRegistry.create())
	{
		if (parameters.Parent != entt::null)
			sceneRegistry.emplace<ParentComponent>(entityID, parameters.Parent);

		sceneRegistry.emplace<TransformComponent>(entityID, parameters.Position);
		sceneRegistry.emplace<SpriteRendererComponent>(entityID, parameters.Size, parameters.Texture, parameters.Color);

		if (parameters.OnClick)
			sceneRegistry.emplace<OnClickCallbackComponent>(entityID, parameters.OnClick);
	}
	Panel::~Panel()
	{
		sceneRegistry.destroy(entityID);
	}
}