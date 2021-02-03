#include "canpch.h"
#include "TransformComponent.h"

namespace Can
{
	TransformComponent::TransformComponent(const glm::vec3& position)
		: Position(position)
	{
	}
	TransformComponent::TransformComponent(const glm::vec3& position, const glm::vec2& scale)
		: Position(position)
		, Scale(scale)
	{
	}
	TransformComponent::TransformComponent(const glm::vec3& position, const glm::vec2& scale, float rotationInRadians)
		: Position(position)
		, Scale(scale)
		, Rotation(rotationInRadians)
	{
	}
}
