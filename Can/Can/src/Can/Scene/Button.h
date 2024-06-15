#pragma once
#include "Can.h"

namespace Can
{
	struct ButtonConstructorParameters
	{
		entt::registry& Registry;
		entt::entity Parent = entt::null;
		glm::vec3 Position{ 0.0f, 0.0f, 0.0f };
		glm::vec2 Size{ 1.0f, 1.0f };
		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };
		Ref<Texture2D> Texture = nullptr;
		std::function<void()> OnClick;
		float borderRadius = 0.0f;
		bool border = false;
		glm::vec4 BorderColor{ 0.0f, 0.0f, 0.0f, 1.0f };
	};
	class Button
	{
	public:
		Button(const ButtonConstructorParameters& parameters);
		~Button();

	public:
		entt::registry& sceneRegistry;
		entt::entity entityID;
	};
}