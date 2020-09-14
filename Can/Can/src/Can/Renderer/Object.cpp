#include "canpch.h"
#include "Object.h"
#include "Can.h"

namespace Can
{
	Object::Object(Prefab* prefab, Prefab* type, bool enabled)
		: prefab(prefab)
		, type(type)
		, position({ 0.0f, 0.0f, 0.0f })
		, scale({ 1.0f, 1.0f, 1.0f })
		, rotation({ 0.0f, 0.0f, 0.0f })
		, q(glm::quat(rotation))
		, transform(glm::translate(glm::mat4(1.0f), position) * glm::mat4_cast(q)* glm::scale(glm::mat4(1.0f), scale))
		, enabled(enabled)
	{
		Renderer3D::AddObject(this);
	}
	Object::Object(Prefab* prefab, Prefab* type, const glm::mat4& transform, bool enabled)
		: prefab(prefab)
		, type(type)
		, transform(transform)
		, enabled(enabled)
	{
		glm::decompose(transform, scale, q, position, skew, perspective);
		rotation = glm::eulerAngles(q);
		Renderer3D::AddObject(this);
	}
	Object::Object(Prefab* prefab, Prefab* type, const glm::vec3& position, bool enabled)
		: prefab(prefab)
		, type(type)
		, position(position)
		, scale({ 1.0f, 1.0f, 1.0f })
		, rotation({ 0.0f, 0.0f, 0.0f })
		, q(glm::quat(rotation))
		, transform(glm::translate(glm::mat4(1.0f), position)* glm::mat4_cast(q)* glm::scale(glm::mat4(1.0f), scale))
		, enabled(enabled)
	{
		Renderer3D::AddObject(this);
	}
	Object::Object(Prefab* prefab, Prefab* type, const glm::vec3& position, const glm::vec3& scale, bool enabled)
		: prefab(prefab)
		, type(type)
		, position(position)
		, scale(scale)
		, rotation({ 0.0f, 0.0f, 0.0f })
		, q(glm::quat(rotation))
		, transform(glm::translate(glm::mat4(1.0f), position)* glm::mat4_cast(q)* glm::scale(glm::mat4(1.0f), scale))
		, enabled(enabled)
	{
	}
	Object::Object(Prefab* prefab, Prefab* type, const glm::vec3& position, const glm::vec3& scale, const glm::vec3& rotation, bool enabled)
		: prefab(prefab)
		, type(type)
		, position(position)
		, scale(scale)
		, rotation(rotation)
		, q(glm::quat(rotation))
		, transform(glm::translate(glm::mat4(1.0f), position)* glm::mat4_cast(q)* glm::scale(glm::mat4(1.0f), scale))
		, enabled(enabled)
	{
		Renderer3D::AddObject(this);
	}
	Object::Object(Prefab* prefab, Prefab* type, const glm::vec3& position, const glm::vec3& scale, const glm::quat& q, bool enabled)
		: prefab(prefab)
		, type(type)
		, position(position)
		, scale(scale)
		, rotation(glm::eulerAngles(q))
		, q(q)
		, transform(glm::translate(glm::mat4(1.0f), position)* glm::mat4_cast(q)* glm::scale(glm::mat4(1.0f), scale))
		, enabled(enabled)
	{
		Renderer3D::AddObject(this);
	}
	Object::~Object()
	{
		Renderer3D::DeleteObject(this);
	}
	void Object::SetTransform(const glm::mat4& transform)
	{
		CAN_PROFILE_FUNCTION();
		
		this->transform = transform;
		glm::decompose(transform, scale, q, position, skew, perspective);
	}
	void Object::SetTransform(const glm::vec3& position)
	{
		SetTransform(position, this->scale, this->rotation);
	}
	void Object::SetTransform(const glm::vec3& position, const glm::vec3& scale)
	{
		SetTransform(position, scale, this->rotation);
	}
	void Object::SetTransform(const glm::vec3& position, const glm::vec3& scale, const glm::vec3& rotation)
	{
		CAN_PROFILE_FUNCTION();

		this->position = position;
		this->scale = scale;
		this->rotation = rotation;
		this->q = glm::quat(this->rotation);

		this->transform = 
			glm::translate(glm::mat4(1.0f), position) * 
			glm::rotate(glm::mat4(1.0f), rotation.y, glm::vec3{ 0.0f, 1.0f, 0.0f }) *
			glm::rotate(glm::mat4(1.0f), rotation.z, glm::vec3{ 0.0f, 0.0f, 1.0f }) *
			glm::rotate(glm::mat4(1.0f), rotation.x, glm::vec3{ 1.0f, 0.0f, 0.0f }) *
			glm::scale(glm::mat4(1.0f), scale);
	}
}