#include "canpch.h"
#include "TransformComponent.h"

namespace Can
{
	TransformComponent::TransformComponent()
		: Transform(glm::mat4(1))
	{
	}
	TransformComponent::TransformComponent(const glm::vec3& position)
		: Transform(glm::translate(glm::mat4(1),position))
	{
	}
	TransformComponent::TransformComponent(const glm::vec3& position, const glm::vec3& scale)
		: Transform(glm::translate(glm::mat4(1), position) *glm::scale(glm::mat4(1),scale))
	{
	}
	TransformComponent::TransformComponent(const glm::vec3& position, const glm::vec3& scale, float rotationInRadians)
		: Transform(glm::translate(glm::mat4(1), position) * glm::rotate(glm::mat4(1.0f), rotationInRadians, glm::vec3(0, 0, 1)) * glm::scale(glm::mat4(1), scale))
	{
	}
	TransformComponent::TransformComponent(const glm::mat4& transform)
		: Transform(transform)
	{
	}
}
