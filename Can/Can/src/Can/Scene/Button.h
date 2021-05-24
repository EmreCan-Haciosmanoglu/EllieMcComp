#pragma once
#include "Can.h"

namespace Can
{
	struct ButtonConstructorParameters
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
		glm::vec4& BorderColor = glm::vec4( 0.0f, 0.0f, 0.0f, 1.0f );
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