#include "canpch.h"
#include "Object.h"

namespace Can
{
	Object::Object(const Ref<Prefab>& prefab, const Ref<Prefab>& type, bool enabled)
		: prefab(prefab)
		, type(type)
		, position({ 0.0f, 0.0f, 0.0f })
		, scale({ 1.0f, 1.0f, 1.0f })
		, transform(glm::mat4(1.0f))
	{
	}
	Object::Object(const Ref<Prefab>& prefab, const Ref<Prefab>& type, const glm::mat4& transform, bool enabled)
		: prefab(prefab)
		, type(type)
		, transform(transform)
		, enabled(enabled)
	{
		glm::decompose(transform, scale, q, position, skew, perspective);
	}
	Object::Object(const Ref<Prefab>& prefab, const Ref<Prefab>& type, const glm::vec3& position, bool enabled)
		: prefab(prefab)
		, type(type)
		, position(position)
		, scale({ 1.0f, 1.0f, 1.0f })
		, transform(glm::translate(glm::mat4(1.0f), position))
		, enabled(enabled)
	{
	}
	Object::Object(const Ref<Prefab>& prefab, const Ref<Prefab>& type, const glm::vec3& position, const glm::vec3& scale, bool enabled)
		: prefab(prefab)
		, type(type)
		, position(position)
		, scale(scale)
		, transform(glm::translate(glm::mat4(1.0f), position)* glm::scale(glm::mat4(1.0f), scale))
		, enabled(enabled)
	{
	}
	Object::Object(const Ref<Prefab>& prefab, const Ref<Prefab>& type, const glm::vec3& position, const glm::vec3& scale, const glm::vec3& rotation, bool enabled)
		: prefab(prefab)
		, type(type)
		, position(position)
		, scale(scale)
		, rotation(rotation)
		, q(glm::quat(rotation))
		, transform(glm::translate(glm::mat4(1.0f), position)* glm::mat4_cast(q)* glm::scale(glm::mat4(1.0f), scale))
		, enabled(enabled)
	{
	}
	Object::Object(const Ref<Prefab>& prefab, const Ref<Prefab>& type, const glm::vec3& position, const glm::vec3& scale, const glm::quat& q, bool enabled)
		: prefab(prefab)
		, type(type)
		, position(position)
		, scale(scale)
		, rotation({ 0.0f, 0.0f, 0.0f })
		, q(q)
		, transform(glm::translate(glm::mat4(1.0f), position)* glm::mat4_cast(q)* glm::scale(glm::mat4(1.0f), scale))
		, enabled(enabled)
	{
		rotation = glm::eulerAngles(q);
	}
	void Object::SetTransform(const glm::mat4& transform)
	{
		this->transform = transform;
		glm::decompose(transform, scale, q, position, skew, perspective);
	}
	void Object::SetTransform(const glm::vec3& position)
	{
		this->position = position;

		this->transform = glm::translate(glm::mat4(1.0f), position) * glm::mat4_cast(q) * glm::scale(glm::mat4(1.0f), scale);
	}
	void Object::SetTransform(const glm::vec3& position, const glm::vec3& scale)
	{
		this->position = position;
		this->scale = scale;

		this->transform = glm::translate(glm::mat4(1.0f), position) * glm::mat4_cast(q) * glm::scale(glm::mat4(1.0f), scale);
	}
	void Object::SetTransform(const glm::vec3& position, const glm::vec3& scale, const glm::vec3& rotation)
	{
		this->position = position;
		this->scale = scale;
		this->rotation = rotation;
		this->q = glm::quat(this->rotation);

		this->transform = glm::translate(glm::mat4(1.0f), position) * glm::mat4_cast(q) * glm::scale(glm::mat4(1.0f), scale);
	}
}