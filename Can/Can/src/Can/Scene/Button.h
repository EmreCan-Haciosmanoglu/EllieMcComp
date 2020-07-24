#pragma once
#include "Can.h"

namespace Can
{
	class Button
	{
	public:
		Button(
			entt::registry& registry,
			entt::entity parent
		);
		Button(
			entt::registry& registry,
			entt::entity parent,
			const glm::vec3& position,
			const glm::vec3& size,
			const glm::vec4& color
		);
		Button(
			entt::registry& registry,
			entt::entity parent,
			const glm::vec3& position,
			const glm::vec3& size,
			const glm::vec4& color,
			std::function<void()> onClick
		);
		Button(
			entt::registry& registry
		);

		Button(
			entt::registry& registry,
			const glm::vec3& position,
			const glm::vec3& size,
			const glm::vec4& color
		);
		Button(
			entt::registry& registry,
			const glm::vec3& position,
			const glm::vec3& size,
			const glm::vec4& color,
			std::function<void()> onClick
		);

	public:
		entt::registry* sceneRegistry;
		entt::entity entityID;
	};
}