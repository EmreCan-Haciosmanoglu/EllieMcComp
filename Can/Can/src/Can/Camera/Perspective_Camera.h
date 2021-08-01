#pragma once

#include <glm/glm.hpp>

#undef far
#undef near


namespace Can
{
	class Perspective_Camera
	{
	public:
		Perspective_Camera(
			f32 fovy,
			f32 aspect,
			f32 n = 0.1f,
			f32 f = 10.0f,
			v3 pos = v3{ 0.0f, 0.0f, 0.0f },
			v3 rot = v3{ 0.0f, 0.0f, 0.0f });

		void set_projection_matrix(f32 fovy, f32 aspect, f32 n = 0.1f, f32 f = 10.0f);
		void set_position(const v3& pos);
		void set_rotation(const v3& rot);

		void recalculate_direction_vectors();

		void recalculate_transform_matrix();

		void recalculate_projection_matrix();
		void recalculate_view_matrix();
		void recalculate_projection_view_matrix();

	public:

		f32 field_of_view_angle = 0.0f;
		f32 aspect_ratio = 0.0f;
		f32 near_clip_plane = 0.0f;
		f32 far_clip_plane = 0.0f;

		v3 position{ 0.0f, 0.0f, 0.0f };
		v3 rotation{ 0.0f, 0.0f, 0.0f };
		m4 transform = m4(1.0f);
		m4 view = glm::mat4(1.0f);
		m4 projection = glm::mat4(1.0f);
		m4 view_projection = glm::mat4(1.0f);

		v3 forward{ 1.0f, 0.0f, 0.0f };
		v3 up{ 0.0f, 0.0f, 1.0f };
		v3 right{ 0.0f, -1.0f, 0.0f };
	};
}