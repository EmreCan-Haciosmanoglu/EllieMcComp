#pragma once
#include "Can.h"
#include <entt/entt.hpp>

namespace Can
{
	class Panel
	{
	public:
		Panel(entt::registry& registry);
		Panel(
			entt::registry& registry,
			const glm::vec3& position,
			const glm::vec3& size,
			const glm::vec4& color
		);
		Panel(
			entt::registry& registry,
			const glm::vec3& position,
			const glm::vec3& size,
			const glm::vec4& color,
			std::function<void()> onClick
		);
		Panel(
			entt::registry& registry,
			const glm::vec3& position,
			const glm::vec3& size,
			const Ref<Texture2D>& texture,
			const glm::vec4& tintColor
		);
		Panel(
			entt::registry& registry,
			const glm::vec3& position,
			const glm::vec3& size,
			const Ref<Texture2D>& texture,
			const glm::vec4& tintColor,
			std::function<void()> onClick
		);

		public:
		entt::registry* sceneRegistry;
		entt::entity entityID;
	};
}