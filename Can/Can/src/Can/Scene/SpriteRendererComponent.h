#pragma once
#include "Can.h"

namespace Can
{
	struct SpriteRendererComponent
	{
		SpriteRendererComponent();
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec2& size, const glm::vec4& color);
		SpriteRendererComponent(const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& tintColor);
		
		operator glm::vec4& () { return color; }
		operator const glm::vec4& () const { return color; }

		operator Ref<Texture2D>& () { return texture; }
		operator const Ref<Texture2D>& () const { return texture; }

		glm::vec2 size;
		glm::vec4 color;
		Ref<Texture2D> texture;
	};
}