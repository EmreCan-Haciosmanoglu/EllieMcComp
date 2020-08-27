#pragma once
#include "Can/Renderer/Prefab.h"
#include <glm/glm.hpp>

namespace Can
{
	class Object
	{
	public:
		Object(Prefab* prefab, Prefab* type, bool enabled = true);
		Object(Prefab* prefab, Prefab* type, const glm::mat4& transform, bool enabled = true);
		Object(Prefab* prefab, Prefab* type, const glm::vec3& position, bool enabled = true);
		Object(Prefab* prefab, Prefab* type, const glm::vec3& position, const glm::vec3& scale, bool enabled = true);
		Object(Prefab* prefab, Prefab* type, const glm::vec3& position, const glm::vec3& scale, const glm::vec3& rotation, bool enabled = true);
		Object(Prefab* prefab, Prefab* type, const glm::vec3& position, const glm::vec3& scale, const glm::quat& q, bool enabled = true);

		~Object();

		void SetTransform(const glm::mat4& transform);
		void SetTransform(const glm::vec3& position);
		void SetTransform(const glm::vec3& position, const glm::vec3& scale);
		void SetTransform(const glm::vec3& position, const glm::vec3& scale, const glm::vec3& rotation);

	public:
		Prefab* prefab;
		Prefab* type;

		glm::vec4 tintColor = { 1.0f, 1.0f, 1.0f, 1.0f };

		glm::vec3 position;
		glm::vec3 scale;
		glm::vec3 rotation;
		glm::quat q;
		glm::vec3 skew;
		glm::vec4 perspective;

		glm::mat4 transform;

		bool enabled;
	};
}