#pragma once
#include "Can.h"

namespace Can
{
	struct SpriteRendererComponent
	{
		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& tintColor, bool border = false, float borderRadius = 0.0f, const glm::vec4& borderColor = { 0.0f, 0.0f, 0.0f, 1.0f });

		operator glm::vec4& () { return color; }
		operator const glm::vec4& () const { return color; }

		operator Ref<Texture2D>& () { return texture; }
		operator const Ref<Texture2D>& () const { return texture; }

		glm::vec2 size = glm::vec2(1.0f);
		glm::vec4 color = glm::vec4(1.0f);
		glm::vec4 bordercolor{ 0.0f, 0.0f, 0.0f, 1.0f };
		Ref<Texture2D> texture = nullptr;

		std::array<float, 4> trim{
			0.0f, // Top
			1.0f, // Right
			1.0f, // Bottom
			0.0f  // Left
		};

		float borderRadius = 0.0f;
		bool border = false;
	};
}