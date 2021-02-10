#include "canpch.h"
#include "SpriteRendererComponent.h"

namespace Can
{
	SpriteRendererComponent::SpriteRendererComponent(const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& tintColor, bool border, float borderRadius)
		: size(size)
		, color(tintColor)
		, texture(texture)
		, border(border)
		, borderRadius(borderRadius)
	{
	}
}