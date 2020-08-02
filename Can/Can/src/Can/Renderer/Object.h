#pragma once
#include "Can.h"

namespace Can
{
	class Object
	{
	public:
		Object(const Ref<Prefab>& prefab, const Ref<Prefab>& type, bool enabled = true);
		Object(const Ref<Prefab>& prefab, const Ref<Prefab>& type, const glm::mat4& transform, bool enabled = true);
		Object(const Ref<Prefab>& prefab, const Ref<Prefab>& type, const glm::vec3& position, bool enabled = true);
		Object(const Ref<Prefab>& prefab, const Ref<Prefab>& type, const glm::vec3& position, const glm::vec3& scale, bool enabled = true);
		Object(const Ref<Prefab>& prefab, const Ref<Prefab>& type, const glm::vec3& position, const glm::vec3& scale, const glm::vec3& rotation, bool enabled = true);
		Object(const Ref<Prefab>& prefab, const Ref<Prefab>& type, const glm::vec3& position, const glm::vec3& scale, const glm::quat& q, bool enabled = true);
		
		~Object() = default;

		void SetTransform(const glm::mat4& transform);
		void SetTransform(const glm::vec3& position);
		void SetTransform(const glm::vec3& position, const glm::vec3& scale);
		void SetTransform(const glm::vec3& position, const glm::vec3& scale, const glm::vec3& rotation);

	public:
		Ref<Prefab> prefab;
		Ref<Prefab> type;

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