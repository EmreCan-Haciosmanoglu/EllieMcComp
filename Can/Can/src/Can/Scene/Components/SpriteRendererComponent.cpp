#include "canpch.h"
#include "SpriteRendererComponent.h"

namespace Can
{
	SpriteRendererComponent::SpriteRendererComponent(const glm::vec2& size, const  glm::vec4& color)
		: size(size)
		, color(color)
	{
	}
	SpriteRendererComponent::SpriteRendererComponent(const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& tintColor)
		: size(size)
		, color(tintColor)
		, texture(texture)
	{
	}
}