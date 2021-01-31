#include "canpch.h"
#include "Button.h"
#include <glm/gtx/matrix_decompose.hpp>

namespace Can
{
	Button::Button(const ButtonConstructorParameters& parameters)
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
}