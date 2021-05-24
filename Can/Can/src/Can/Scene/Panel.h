#pragma once
#include "Can.h"
#include <entt/entt.hpp>

namespace Can
{
	struct PanelConstructorParameters
	{
		entt::registry& Registry;
		entt::entity Parent = entt::null;
		glm::vec3& Position = glm::vec3(0.0f);
		glm::vec2& Size = glm::vec2(1.0f);
		glm::vec4& Color = glm::vec4(1.0f);
		Ref<Texture2D> Texture = nullptr;
		std::function<void()> OnClick;
		float borderRadius = 0.0f;
		bool border = false;
		const glm::vec4& BorderColor{ 0.0f, 0.0f, 0.0f, 1.0f };
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