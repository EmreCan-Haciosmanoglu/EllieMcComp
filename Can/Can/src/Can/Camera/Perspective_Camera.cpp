#include "canpch.h"
#include "Perspective_Camera.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Can
{


	Perspective_Camera::Perspective_Camera(f32 fovy, f32 aspect, f32 n, f32 f, v3 pos, v3 rot)
		: field_of_view_angle(fovy)
		, aspect_ratio(aspect)
		, near_clip_plane(n)
		, far_clip_plane(f)
		, position(pos)
		, rotation(rot)
		, transform(glm::translate(m4(1.0f), position)*
			glm::rotate(m4(1.0f), rotation.z, v3{ 0.0f, 0.0f, 1.0f })*
			glm::rotate(m4(1.0f), rotation.y, v3{ 0.0f, 1.0f, 0.0f })*
			glm::rotate(m4(1.0f), rotation.x, v3{ 1.0f, 0.0f, 0.0f }))
		, view(glm::inverse(transform))
		, projection(glm::perspective(field_of_view_angle, aspect, near_clip_plane, far_clip_plane))
		, view_projection(projection * view)
	{
		recalculate_direction_vectors();
	}

	void Perspective_Camera::set_projection_matrix(f32 fov, f32 aspect, f32 n, f32 f)
	{
		CAN_PROFILE_FUNCTION();

		field_of_view_angle = fov;
		aspect_ratio = aspect;
		near_clip_plane = n;
		far_clip_plane = f;

		recalculate_projection_matrix();
	}

	void Perspective_Camera::set_position(const v3& pos)
	{
		CAN_PROFILE_FUNCTION();
		transform = glm::translate(m4(1.0f), pos - position) * transform;
		position = pos;

		recalculate_view_matrix();
	}

	void Perspective_Camera::set_rotation(const v3& rot)
	{
		CAN_PROFILE_FUNCTION();
		rotation = rot;
		recalculate_direction_vectors();
		recalculate_transform_matrix();
	}


	void Perspective_Camera::recalculate_direction_vectors()
	{
		CAN_PROFILE_FUNCTION();
		v3 rotation_in_radians{
			glm::radians(rotation.x),
			glm::radians(rotation.y),
			glm::radians(rotation.z)
		};

		forward = {
			glm::cos(rotation_in_radians.y) * glm::cos(rotation_in_radians.z),
			glm::sin(rotation_in_radians.z),
			glm::sin(rotation_in_radians.y)
		};
		up = {
			glm::sin(rotation_in_radians.y) * glm::cos(rotation_in_radians.z + glm::radians(180.0f)),
			glm::sin(rotation_in_radians.y) * glm::sin(rotation_in_radians.z + glm::radians(180.0f)),
			glm::cos(rotation_in_radians.y)
		};
		right = glm::cross(forward, up);
	}

	void Perspective_Camera::recalculate_transform_matrix()
	{
		transform =
			glm::translate(m4(1.0f), position) *
			glm::rotate(m4(1.0f), rotation.z, v3{ 0.0f, 0.0f, 1.0f }) *
			glm::rotate(m4(1.0f), rotation.y, v3{ 0.0f, 1.0f, 0.0f }) *
			glm::rotate(m4(1.0f), rotation.x, v3{ 1.0f, 0.0f, 0.0f });
		recalculate_view_matrix();
	}

	void Perspective_Camera::recalculate_projection_matrix()
	{
		projection = glm::perspective(field_of_view_angle, aspect_ratio, near_clip_plane, far_clip_plane);
		recalculate_projection_view_matrix();
	}
	void Perspective_Camera::recalculate_view_matrix()
	{
		view = glm::inverse(transform);
		recalculate_projection_view_matrix();
	}
	void Perspective_Camera::recalculate_projection_view_matrix()
	{
		CAN_PROFILE_FUNCTION();
		view_projection = projection * view;
	}
}