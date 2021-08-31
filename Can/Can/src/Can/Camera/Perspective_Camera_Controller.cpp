#include "canpch.h"
#include "Perspective_Camera_Controller.h"

#include "Can/Math.h"

namespace Can
{

	Perspective_Camera_Controller::Perspective_Camera_Controller(
		f32 fovy, f32 aspect, f32 n, f32 f, v3 pos, v3 rot
	) : camera(fovy, aspect, n, f, pos, rot) {	}

	void Perspective_Camera_Controller::on_update(Can::TimeStep ts)
	{
		CAN_PROFILE_FUNCTION();

		f32 frame_independent_translation_speed = translation_speed * ts;
		f32 frame_independent_rotation_speed = rotation_speed * ts; // different speeds for pitching and yawing?
		f32 frame_independent_rotation_speed_in_radians = glm::radians(frame_independent_rotation_speed);

		if (Input::IsKeyPressed(left_key))
			translate_relative(v3{ 0.0f, -1.0f, 0.0f }, frame_independent_translation_speed);
		else if (Input::IsKeyPressed(right_key))
			translate_relative(v3{ 0.0f, +1.0f, 0.0f }, frame_independent_translation_speed);

		if (Input::IsKeyPressed(forward_key))
			translate_relative(v3{ +1.0f, 0.0f, 0.0f }, frame_independent_translation_speed);
		else if (Input::IsKeyPressed(backward_key))
			translate_relative(v3{ -1.0f, 0.0f, 0.0f }, frame_independent_translation_speed);

		if (Input::IsKeyPressed(rotate_cw_key))
		{
			v3 forward = camera.forward;
			v3 center = Math::RayPlaneIntersection(
				camera.position,
				forward,
				v3{ 0.0f, 0.0f, 0.0f },
				v3{ 0.0f, 0.0f, 1.0f }
			);
			center = glm::clamp(center, camera.position - v3{ 10.0f, 10.0f, 10.0f }, camera.position + v3{ 10.0f, 10.0f, 10.0f });
			orbit_around_point(frame_independent_rotation_speed_in_radians, center, false);
		}
		else if (Input::IsKeyPressed(rotate_ccw_key))
		{
			v3 forward = camera.forward;
			v3 center = Math::RayPlaneIntersection(
				camera.position,
				forward,
				v3{ 0.0f, 0.0f, 0.0f },
				v3{ 0.0f, 0.0f, 1.0f }
			);
			center = glm::clamp(center, camera.position - v3{ 10.0f, 10.0f, 10.0f }, camera.position + v3{ 10.0f, 10.0f, 10.0f });
			orbit_around_point(frame_independent_rotation_speed_in_radians, center, true);
		}

		if (Input::IsKeyPressed(lower_key))
			translate_relative(v3{ 0.0f, 0.0f, -1.0f }, frame_independent_translation_speed);
		else if (Input::IsKeyPressed(raise_key))
			translate_relative(v3{ 0.0f, 0.0f, +1.0f }, frame_independent_translation_speed);


		if (Input::IsKeyPressed(pitch_down_key))
			pitch(frame_independent_rotation_speed, false);
		else if (Input::IsKeyPressed(pitch_up_key))
			pitch(frame_independent_rotation_speed, true);
		camera.rotation = {
			0.0f,
			glm::clamp(camera.rotation.y, -90.0f, -10.0f),
			std::fmod(camera.rotation.z + 360.0f, 360.0f)
		};
		camera.recalculate_direction_vectors();
		camera.recalculate_view_matrix();

		if (Input::IsKeyPressed(increase_fov_key))
			camera.field_of_view_angle += 2.0f * ts;
		else if (Input::IsKeyPressed(decrease_fov_key))
			camera.field_of_view_angle -= 2.0f * ts;
		camera.recalculate_projection_matrix();
	}
	void Perspective_Camera_Controller::on_event(Event::Event& e)
	{
		CAN_PROFILE_FUNCTION();

		Event::EventDispatcher dispatcher(e);
		dispatcher.Dispatch< Event::WindowResizeEvent>(CAN_BIND_EVENT_FN(Perspective_Camera_Controller::on_window_resized));
	}

	void Perspective_Camera_Controller::translate(v3 direction, f32 length)
	{
		f32 angle_in_radians = glm::radians(camera.rotation.z);
		v3 sized = direction * length;
		camera.position += sized;

		camera.set_position(camera.position);
	}
	void Perspective_Camera_Controller::translate_relative(v3 direction, f32 length)
	{
		f32 angle_in_radians = glm::radians(camera.rotation.z);
		v3 sized = direction * length;
		v3 aligned_and_sized = glm::rotateZ(sized, angle_in_radians);
		camera.position += aligned_and_sized;

		camera.set_position(camera.position);
	}
	
	void Perspective_Camera_Controller::pitch(f32 amount_in_radians, bool clockwise)
	{
		camera.rotation.y += amount_in_radians * (clockwise * 2.0f - 1.0f);
		camera.recalculate_direction_vectors();
		camera.recalculate_view_matrix();
	}
	void Perspective_Camera_Controller::orbit_around_point(f32 amount_in_radians, v3 point, bool clockwise)
	{
		v3 difference = camera.position - point;
		f32 yaw_in_radians = amount_in_radians * (1.0f - 2.0f * clockwise);
		camera.rotation.z += glm::degrees(yaw_in_radians);
		difference = glm::rotateZ(difference, yaw_in_radians);
		camera.set_rotation(camera.rotation);
		camera.set_position(point + difference);
	}

	bool Perspective_Camera_Controller::on_window_resized(Event::WindowResizeEvent& event)
	{
		CAN_PROFILE_FUNCTION();

		if (event.width == 0 || event.height == 0)
			return false;
		camera.aspect_ratio = (f32)event.width / (f32)event.height;
		camera.recalculate_projection_matrix();
		return false;
	}
}