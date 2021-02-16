#include "canpch.h"
#include "SpriteRendererComponent.h"

namespace Can
{
	SpriteRendererComponent::SpriteRendererComponent(const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& tintColor, bool border, float borderRadius, const glm::vec4& borderColor)
		: size(size)
		, color(tintColor)
		, texture(texture)
		, bordercolor(borderColor)
		, border(border)
		, borderRadius(borderRadius)
	{
	}
}