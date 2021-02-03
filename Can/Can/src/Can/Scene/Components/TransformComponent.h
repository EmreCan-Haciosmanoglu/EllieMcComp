#pragma once
#include "Can.h"
namespace Can
{
	struct TransformComponent
	{
		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& position);
		TransformComponent(const glm::vec3& position, const glm::vec2& scale);
		TransformComponent(const glm::vec3& position, const glm::vec2& scale, float rotation);

		operator glm::mat4& () { return glm::translate(glm::mat4(1), Position) * glm::rotate(glm::mat4(1.0f), Rotation, glm::vec3(0, 0, 1)) * glm::scale(glm::mat4(1), glm::vec3(Scale, 1.0f)); }
		operator const glm::mat4& () const { return glm::translate(glm::mat4(1), Position) * glm::rotate(glm::mat4(1.0f), Rotation, glm::vec3(0.0f, 0.0f, 1.0f)) * glm::scale(glm::mat4(1), glm::vec3(Scale, 1.0f)); }

		glm::vec3 Position = glm::vec3(0.0f);
		glm::vec2 Scale = glm::vec2(1.0f);
		float Rotation = 0.0f;
	};
}