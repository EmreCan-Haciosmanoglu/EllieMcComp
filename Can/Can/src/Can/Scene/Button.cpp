#include "canpch.h"
#include "Button.h"
#include <glm/gtx/matrix_decompose.hpp>

namespace Can
{
	Button::Button(entt::registry& registry)
		: sceneRegistry(&registry)
		, entityID(sceneRegistry->create())
	{
		sceneRegistry->emplace<TransformComponent>(entityID);
		sceneRegistry->emplace<SpriteRendererComponent>(entityID);
	}
	Button::Button(entt::registry& registry, const glm::vec3& position, const glm::vec3& size, const glm::vec4& color)
		: sceneRegistry(&registry)
		, entityID(sceneRegistry->create())
	{
		sceneRegistry->emplace<TransformComponent>(entityID, position);
		sceneRegistry->emplace<SpriteRendererComponent>(entityID, size, color);
	}
	Button::Button(entt::registry& registry, const glm::vec3& position, const glm::vec3& size, const glm::vec4& color, std::function<void()> onClick)
		: sceneRegistry(&registry)
		, entityID(sceneRegistry->create())
	{
		sceneRegistry->emplace<TransformComponent>(entityID, position);
		sceneRegistry->emplace<SpriteRendererComponent>(entityID, size, color);
		sceneRegistry->emplace<OnClickCallbackComponent>(entityID, onClick);
	}
}