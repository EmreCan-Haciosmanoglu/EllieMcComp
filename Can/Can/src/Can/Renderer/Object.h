#pragma once
#include "Can/Renderer/Prefab.h"
#include <glm/glm.hpp>

namespace Can
{
	class Object
	{
	public:
		Object(Prefab* prefab);
		Object(Prefab* prefab, const m4& transform );
		Object(Prefab* prefab, const v3& position, const v3& rotation = v3(0.0f), const v3& scale = v3(1.0f));
		Object(Prefab* prefab, const v3& position, const glm::quat& q, const v3& scale = v3(1.0f) );
		Object(Object&& other);
		~Object();

		void SetTransform(const m4& transform);
		void SetTransform(const v3& position);
		void SetTransform(const v3& position, const v3& rotation);
		void SetTransform(const v3& position, const v3& rotation, const v3& scale);

	public:
		Prefab* prefab = nullptr;
		v3 position = v3(0.0f);
		v3 rotation = v3(0.0f);
		v3 scale = v3(1.0f);

		v4 tintColor = { 1.0f, 1.0f, 1.0f, 1.0f };

		glm::quat q;
		v3 skew = v3(0.0f);
		v4 perspective = v4(0.0f);

		m4 transform;

		bool enabled = true;
		bool owns_prefab = false;
	};
}