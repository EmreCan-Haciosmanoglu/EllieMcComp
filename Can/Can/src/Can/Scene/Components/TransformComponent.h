#pragma once
#include "Can.h"
namespace Can
{
	struct TransformComponent
	{
		TransformComponent();
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& position);
		TransformComponent(const glm::vec3& position, const glm::vec3& scale);
		TransformComponent(const glm::vec3& position, const glm::vec3& scale, float rotation);
		TransformComponent(const glm::mat4& transform);

		operator glm::mat4& () { return Transform; }
		operator const glm::mat4& () const { return Transform; }

		glm::mat4 Transform;
	};
}