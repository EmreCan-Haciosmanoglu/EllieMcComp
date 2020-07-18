#include "canpch.h"
#include "SpriteRendererComponent.h"

namespace Can
{
	SpriteRendererComponent::SpriteRendererComponent()
		: size({ 1.0f, 1.0f })
		, color({ 1.0f, 1.0f, 1.0f, 1.0f })
	{
	}
	SpriteRendererComponent::SpriteRendererComponent(const glm::vec2& size, glm::vec4& color)
		: size(size)
		, color(color)
	{
	}
	SpriteRendererComponent::SpriteRendererComponent(const glm::vec2& size, const Ref<Texture2D>& texture, glm::vec4& tintColor)
		: size(size)
		, color(tintColor)
		, texture(texture)
	{
	}
}