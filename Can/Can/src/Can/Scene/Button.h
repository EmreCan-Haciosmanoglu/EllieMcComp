#pragma once
#include "Can.h"

namespace Can
{
	struct ButtonConstructorParameters
	{
		entt::registry& Registry;
		entt::entity Parent = entt::null;
		const glm::vec3& Position = glm::vec3(0.0f);
		const glm::vec3& Size = glm::vec3(1.0f);
		const glm::vec4& Color = glm::vec4(1.0f);
		const Ref<Texture2D>& Texture = nullptr;
		std::function<void()> OnClick;
	};
	class Button
	{
	public:
		Button(const ButtonConstructorParameters& parameters);

	public:
		entt::registry& sceneRegistry;
		entt::entity entityID;
	};
}