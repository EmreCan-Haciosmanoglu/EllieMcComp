#include "canpch.h"
#include "Object.h"
#include "Can.h"

namespace Can
{
	Object::Object(Prefab* prefab)
		: prefab(prefab)
		, q(glm::quat(rotation))
		, transform(m4(1.0f))
	{
		Renderer3D::AddObject(this);
	}
	Object::Object(Prefab* prefab, const m4& transform)
		: prefab(prefab)
		, transform(transform)
	{
		glm::decompose(transform, scale, q, position, skew, perspective);
		rotation = glm::eulerAngles(q);
		Renderer3D::AddObject(this);
	}
	Object::Object(Prefab* prefab, const v3& position, const v3& rotation, const v3& scale)
		: prefab(prefab)
		, position(position)
		, rotation(rotation)
		, scale(scale)
		, q(glm::quat(rotation))
		, transform(glm::translate(m4(1.0f), position)* glm::mat4_cast(q)* glm::scale(m4(1.0f), scale))
	{
		Renderer3D::AddObject(this);
	}
	Object::Object(Prefab* prefab, const v3& position, const glm::quat& q, const v3& scale)
		: prefab(prefab)
		, position(position)
		, scale(scale)
		, rotation(glm::eulerAngles(q))
		, q(q)
		, transform(glm::translate(m4(1.0f), position)* glm::mat4_cast(q)* glm::scale(m4(1.0f), scale))
	{
		Renderer3D::AddObject(this);
	}
	Object::Object(Object&& other)
	{
		prefab = other.prefab;
		position = other.position;
		rotation = other.rotation;
		scale = other.scale;
		tintColor = other.tintColor;
		q = other.q;
		skew = other.skew;
		perspective = other.perspective;
		transform = other.transform;
		enabled = other.enabled;
		owns_prefab = other.owns_prefab;

		other.prefab = nullptr;
	}
	Object::~Object()
	{
		if (owns_prefab) delete prefab;
		Renderer3D::DeleteObject(this);
	}

	void Object::SetTransform(const m4& transform)
	{
		CAN_PROFILE_FUNCTION();

		this->transform = transform;
		glm::decompose(transform, scale, q, position, skew, perspective);
	}
	void Object::SetTransform(const v3& position)
	{
		SetTransform(position, this->rotation, this->scale);
	}
	void Object::SetTransform(const v3& position, const v3& rotation)
	{
		SetTransform(position, rotation, this->scale);
	}
	void Object::SetTransform(const v3& position, const v3& rotation, const v3& scale)
	{
		CAN_PROFILE_FUNCTION();

		this->position = position;
		this->rotation = rotation;
		this->scale = scale;
		this->q = glm::quat(this->rotation);

		this->transform =
			glm::translate(m4(1.0f), position) *
			glm::rotate(m4(1.0f), rotation.z, v3{ 0.0f, 0.0f, 1.0f }) *
			glm::rotate(m4(1.0f), rotation.y, v3{ 0.0f, 1.0f, 0.0f }) *
			glm::rotate(m4(1.0f), rotation.x, v3{ 1.0f, 0.0f, 0.0f }) *
			glm::scale(m4(1.0f), scale);
	}
}