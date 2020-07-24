#include "canpch.h"
#include "Panel.h"
#include <glm/gtx/matrix_decompose.hpp>

namespace Can
{
	Panel::Panel(
		entt::registry& registry,
		entt::entity parent
	)
		: sceneRegistry(&registry)
		, entityID(sceneRegistry->create())
	{
		registry.emplace<ParentComponent>(entityID, parent);
		sceneRegistry->emplace<TransformComponent>(entityID);
		sceneRegistry->emplace<SpriteRendererComponent>(entityID);
	}

	Panel::Panel(
		entt::registry& registry,
		entt::entity parent,
		const glm::vec3& position,
		const glm::vec3& size,
		const glm::vec4& color
	)
		: sceneRegistry(&registry)
		, entityID(sceneRegistry->create())
	{
		registry.emplace<ParentComponent>(entityID, parent);
		sceneRegistry->emplace<TransformComponent>(entityID, position);
		sceneRegistry->emplace<SpriteRendererComponent>(entityID, size, color);
	}

	Panel::Panel(
		entt::registry& registry,
		entt::entity parent,
		const glm::vec3& position,
		const glm::vec3& size,
		const glm::vec4& color,
		std::function<void()> onClick
	)
		: sceneRegistry(&registry)
		, entityID(sceneRegistry->create())
	{
		registry.emplace<ParentComponent>(entityID, parent);
		sceneRegistry->emplace<TransformComponent>(entityID, position);
		sceneRegistry->emplace<SpriteRendererComponent>(entityID, size, color);
		sceneRegistry->emplace<OnClickCallbackComponent>(entityID, onClick);
	}

	Panel::Panel(
		entt::registry& registry,
		entt::entity parent,
		const glm::vec3& position,
		const glm::vec3& size,
		const Ref<Texture2D>& texture,
		const glm::vec4& tintColor
	)
		: sceneRegistry(&registry)
		, entityID(sceneRegistry->create())
	{
		registry.emplace<ParentComponent>(entityID, parent);
		sceneRegistry->emplace<TransformComponent>(entityID, position);
		sceneRegistry->emplace<SpriteRendererComponent>(entityID, size, texture, tintColor);
	}

	Panel::Panel(
		entt::registry& registry,
		entt::entity parent,
		const glm::vec3& position,
		const glm::vec3& size,
		const Ref<Texture2D>& texture,
		const glm::vec4& tintColor,
		std::function<void()> onClick
	)
		: sceneRegistry(&registry)
		, entityID(sceneRegistry->create())
	{
		registry.emplace<ParentComponent>(entityID, parent);
		sceneRegistry->emplace<TransformComponent>(entityID, position);
		sceneRegistry->emplace<SpriteRendererComponent>(entityID, size, texture, tintColor);
		sceneRegistry->emplace<OnClickCallbackComponent>(entityID, onClick);
	}
	Panel::Panel(
		entt::registry& registry
	)
		: sceneRegistry(&registry)
		, entityID(sceneRegistry->create())
	{
		sceneRegistry->emplace<TransformComponent>(entityID);
		sceneRegistry->emplace<SpriteRendererComponent>(entityID);
	}

	Panel::Panel(
		entt::registry& registry,
		const glm::vec3& position,
		const glm::vec3& size,
		const glm::vec4& color
	)
		: sceneRegistry(&registry)
		, entityID(sceneRegistry->create())
	{
		sceneRegistry->emplace<TransformComponent>(entityID, position);
		sceneRegistry->emplace<SpriteRendererComponent>(entityID, size, color);
	}

	Panel::Panel(
		entt::registry& registry,
		const glm::vec3& position,
		const glm::vec3& size,
		const glm::vec4& color,
		std::function<void()> onClick
	)
		: sceneRegistry(&registry)
		, entityID(sceneRegistry->create())
	{
		sceneRegistry->emplace<TransformComponent>(entityID, position);
		sceneRegistry->emplace<SpriteRendererComponent>(entityID, size, color);
		sceneRegistry->emplace<OnClickCallbackComponent>(entityID, onClick);
	}

	Panel::Panel(
		entt::registry& registry,
		const glm::vec3& position,
		const glm::vec3& size,
		const Ref<Texture2D>& texture,
		const glm::vec4& tintColor
	)
		: sceneRegistry(&registry)
		, entityID(sceneRegistry->create())
	{
		sceneRegistry->emplace<TransformComponent>(entityID, position);
		sceneRegistry->emplace<SpriteRendererComponent>(entityID, size, texture, tintColor);
	}

	Panel::Panel(
		entt::registry& registry,
		const glm::vec3& position,
		const glm::vec3& size,
		const Ref<Texture2D>& texture,
		const glm::vec4& tintColor,
		std::function<void()> onClick
	)
		: sceneRegistry(&registry)
		, entityID(sceneRegistry->create())
	{
		sceneRegistry->emplace<TransformComponent>(entityID, position);
		sceneRegistry->emplace<SpriteRendererComponent>(entityID, size, texture, tintColor);
		sceneRegistry->emplace<OnClickCallbackComponent>(entityID, onClick);
	}
}