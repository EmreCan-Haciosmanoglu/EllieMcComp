#pragma once
#include "Can/Renderer/Prefab.h"
#include <glm/glm.hpp>

namespace Can
{
	class Object
	{
	public:
		Object(Prefab* prefab);
		Object(Prefab* prefab, const glm::mat4& transform );
		Object(Prefab* prefab, const glm::vec3& position, const glm::vec3& rotation = glm::vec3(0.0f), const glm::vec3& scale = glm::vec3(1.0f));
		Object(Prefab* prefab, const glm::vec3& position, const glm::quat& q, const glm::vec3& scale = glm::vec3(1.0f) );
		Object(Object&& other);
		~Object();

		void SetTransform(const glm::mat4& transform);
		void SetTransform(const glm::vec3& position);
		void SetTransform(const glm::vec3& position, const glm::vec3& rotation);
		void SetTransform(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);

	public:
		Prefab* prefab = nullptr;
		glm::vec3 position = glm::vec3(0.0f);
		glm::vec3 rotation = glm::vec3(0.0f);
		glm::vec3 scale = glm::vec3(1.0f);

		glm::vec4 tintColor = { 1.0f, 1.0f, 1.0f, 1.0f };

		glm::quat q;
		glm::vec3 skew = glm::vec3(0.0f);
		glm::vec4 perspective = glm::vec4(0.0f);

		glm::mat4 transform;

		bool enabled = true;
		bool owns_prefab = false;
	};
}