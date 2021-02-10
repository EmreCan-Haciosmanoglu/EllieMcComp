#pragma once
#include "Can.h"
#include <entt/entt.hpp>

namespace Can
{
	struct PanelConstructorParameters
	{
		entt::registry& Registry;
		entt::entity Parent = entt::null;
		const glm::vec3& Position = glm::vec3(0.0f);
		const glm::vec2& Size = glm::vec3(1.0f);
		const glm::vec4& Color = glm::vec4(1.0f);
		const Ref<Texture2D>& Texture = nullptr;
		std::function<void()> OnClick;
		float borderRadius = 0.0f;
		bool border = false;
	};

	class Panel
	{
	public:
		Panel(const PanelConstructorParameters& parameters);
		~Panel();

	public:
		entt::registry& sceneRegistry;
		entt::entity entityID;
	};
}