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
		if (mode == Mode::FollowFirstPerson || mode == Mode::FollowThirdPerson)
			if (follow_object == nullptr)
				mode = Mode::GamePlay;

		f32 frame_independent_translation_speed = translation_speed * ts;
		f32 frame_independent_rotation_speed = rotation_speed * ts; // different speeds for pitching and yawing?

		if (mode == Mode::FollowFirstPerson || mode == Mode::FollowThirdPerson)
		{
			if (
				Input::IsKeyPressed(forward_key) ||
				Input::IsKeyPressed(backward_key) ||
				Input::IsKeyPressed(left_key) ||
				Input::IsKeyPressed(right_key) ||
				Input::IsKeyPressed(rotate_cw_key) ||
				Input::IsKeyPressed(rotate_ccw_key) ||
				Input::IsKeyPressed(pitch_down_key) ||
				Input::IsKeyPressed(pitch_up_key)
				)
			{
				mode = Mode::GamePlay;
			}
		}
		if (mode == Mode::GamePlay)
		{
			if (Input::IsKeyPressed(left_key))
				translate_relative(v3{ 0.0f, +1.0f, 0.0f }, frame_independent_translation_speed);
			else if (Input::IsKeyPressed(right_key))
				translate_relative(v3{ 0.0f, -1.0f, 0.0f }, frame_independent_translation_speed);

			if (Input::IsKeyPressed(forward_key))
				translate_relative(v3{ +1.0f, 0.0f, 0.0f }, frame_independent_translation_speed);
			else if (Input::IsKeyPressed(backward_key))
				translate_relative(v3{ -1.0f, 0.0f, 0.0f }, frame_independent_translation_speed);

			if (Input::IsKeyPressed(rotate_cw_key))
			{
				v3 forward = camera.forward;
				v3 c = Math::ray_plane_intersection(
					camera.position,
					forward,
					v3{ 0.0f, 0.0f, 0.0f },
					v3{ 0.0f, 0.0f, 1.0f }
				);
				c = glm::clamp(c, camera.position - v3{ 10.0f, 10.0f, 10.0f }, camera.position + v3{ 10.0f, 10.0f, 10.0f });
				orbit_around_point(frame_independent_rotation_speed, c, false);
			}
			else if (Input::IsKeyPressed(rotate_ccw_key))
			{
				v3 forward = camera.forward;
				v3 c = Math::ray_plane_intersection(
					camera.position,
					forward,
					v3{ 0.0f, 0.0f, 0.0f },
					v3{ 0.0f, 0.0f, 1.0f }
				);
				c = glm::clamp(c, camera.position - v3{ 10.0f, 10.0f, 10.0f }, camera.position + v3{ 10.0f, 10.0f, 10.0f });
				orbit_around_point(frame_independent_rotation_speed, c, true);
			}

			if (Input::IsKeyPressed(pitch_down_key))
				pitch(frame_independent_rotation_speed, false);
			else if (Input::IsKeyPressed(pitch_up_key))
				pitch(frame_independent_rotation_speed, true);

			center_rot = {
				0.0f,
				glm::clamp(center_rot.y, min_rot_y, max_rot_y),
				std::fmod(center_rot.z + 360.0f, 360.0f)
			};
			camera.set_rotation({
				center_rot.x,
				center_rot.y + temp_additional_rotation_y,
				center_rot.z + temp_additional_rotation_z
				});

		}
		else if (mode == Mode::FreeMoving)
		{
			if (Input::IsKeyPressed(right_key))
				translate(camera.right, frame_independent_translation_speed);
			else if (Input::IsKeyPressed(left_key))
				translate(-camera.right, frame_independent_translation_speed);

			if (Input::IsKeyPressed(forward_key))
				translate(camera.forward, frame_independent_translation_speed);
			else if (Input::IsKeyPressed(backward_key))
				translate(-camera.forward, frame_independent_translation_speed);

			if (Input::IsKeyPressed(raise_key))
				translate(camera.up, frame_independent_translation_speed);
			else if (Input::IsKeyPressed(lower_key))
				translate(-camera.up, frame_independent_translation_speed);

			if (Input::IsKeyPressed(rotate_cw_key))
			{
				camera.rotation.z += frame_independent_rotation_speed;
				camera.set_rotation(camera.rotation);
			}
			else if (Input::IsKeyPressed(rotate_ccw_key))
			{
				camera.rotation.z -= frame_independent_rotation_speed;
				camera.set_rotation(camera.rotation);
			}

			if (Input::IsKeyPressed(pitch_up_key))
			{
				camera.rotation.y += frame_independent_rotation_speed;
				camera.set_rotation(camera.rotation);
			}
			else if (Input::IsKeyPressed(pitch_down_key))
			{
				camera.rotation.y -= frame_independent_rotation_speed;
				camera.set_rotation(camera.rotation);
			}

			camera.set_rotation(camera.rotation);
		}


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
		dispatcher.Dispatch<Event::MouseMovedEvent>(CAN_BIND_EVENT_FN(Perspective_Camera_Controller::on_mouse_moved));
		dispatcher.Dispatch<Event::MouseScrolledEvent>(CAN_BIND_EVENT_FN(Perspective_Camera_Controller::on_mouse_scrolled));
		dispatcher.Dispatch<Event::MouseButtonPressedEvent>(CAN_BIND_EVENT_FN(Perspective_Camera_Controller::on_mouse_pressed));
		dispatcher.Dispatch<Event::MouseButtonReleasedEvent>(CAN_BIND_EVENT_FN(Perspective_Camera_Controller::on_mouse_released));
	}

	void Perspective_Camera_Controller::translate(v3 direction, f32 length)
	{
		v3 sized = direction * length;
		camera.set_position(camera.position + sized);
	}
	void Perspective_Camera_Controller::translate_relative(v3 direction, f32 length)
	{
		f32 angle_in_radians = glm::radians(center_rot.z + temp_additional_rotation_z);
		v3 sized = direction * length;
		v3 aligned_and_sized = glm::rotateZ(sized, angle_in_radians);
		center_pos += aligned_and_sized;
		update_camera_position();
	}

	void Perspective_Camera_Controller::update_camera_position()
	{
		f32 z_angle_in_radians = glm::radians(center_rot.z + temp_additional_rotation_z);
		f32 y_angle_in_radians = glm::radians(center_rot.y + temp_additional_rotation_y);

		f32 lerped_length = Math::lerp(min_pos_z, max_pos_z, zoom_t * zoom_t);
		v3 offset{ lerped_length, 0.0f, 0.0f };
		offset = glm::rotateY(offset, y_angle_in_radians);
		offset = glm::rotateZ(offset, z_angle_in_radians + glm::radians(180.0f));

		camera.set_position(center_pos + offset);
	}

	void Perspective_Camera_Controller::pitch(f32 amount, bool clockwise)
	{
		center_rot.y += amount * (clockwise * 2.0f - 1.0f);
		camera.set_rotation({
			center_rot.x,
			center_rot.y + temp_additional_rotation_y,
			center_rot.z + temp_additional_rotation_z
			});
		update_camera_position();
	}
	void Perspective_Camera_Controller::orbit_around_point(f32 amount, v3 point, bool clockwise)
	{
		center_rot.z += amount * (1.0f - 2.0f * clockwise);
		camera.set_rotation({
			center_rot.x,
			center_rot.y + temp_additional_rotation_y,
			center_rot.z + temp_additional_rotation_z
			});
		update_camera_position();
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

	bool Perspective_Camera_Controller::on_mouse_moved(Event::MouseMovedEvent& event) {
		if (mode == Mode::FollowFirstPerson)
			return false;

		if (is_mouse_dragging)
		{
			auto [mouse_x, mouse_y] = Can::Input::get_mouse_pos_float();

			f32 x_diff = mouse_x - mouse_drag_start_pos.x;

			f32 y_diff = mouse_drag_start_pos.y - mouse_y;

			if (is_y_inverted)
				y_diff = -y_diff;

			temp_additional_rotation_z = x_diff * rotation_with_mouse_speed_multiplier_z;
			temp_additional_rotation_y = y_diff * rotation_with_mouse_speed_multiplier_y;
			if (mode == Mode::GamePlay || mode == Mode::FollowThirdPerson)
			{
				if (center_rot.y + temp_additional_rotation_y > max_rot_y)
				{
					temp_additional_rotation_y = max_rot_y - center_rot.y;
				}
				else if (center_rot.y + temp_additional_rotation_y < min_rot_y)
				{
					temp_additional_rotation_y = min_rot_y - center_rot.y;
				}
				update_camera_position();
			}
			else if (mode == Mode::FreeMoving)
			{
				camera.rotation.z -= temp_additional_rotation_z;
				camera.rotation.y += temp_additional_rotation_y;
				mouse_drag_start_pos = { mouse_x, mouse_y };
				camera.set_rotation(camera.rotation);
			}

		}
		return false;
	}
	bool Perspective_Camera_Controller::on_mouse_scrolled(Event::MouseScrolledEvent& event)
	{
		if (mode == Mode::FollowThirdPerson || mode == Mode::GamePlay)
		{
			f32 x_scroll = event.GetXOffset();
			f32 y_scroll = event.GetYOffset();

			zoom_t -= zoom_speed * y_scroll;
			zoom_t = (glm::min)(zoom_t, 1.0f);
			zoom_t = (glm::max)(zoom_t, 0.0f);
			update_camera_position();
		}
		return false;
	}
	bool Perspective_Camera_Controller::on_mouse_pressed(Event::MouseButtonPressedEvent& event)
	{
		MouseCode key_code = event.GetMouseButton();
		switch (key_code)
		{
		case Can::MouseCode::ButtonLeft: // 0
			if (is_mouse_dragging == false)
			{
				is_mouse_dragging = true;
				auto [mouseX, mouseY] = Can::Input::get_mouse_pos_float();
				mouse_drag_start_pos = { mouseX, mouseY };
			}
			break;
		case Can::MouseCode::ButtonRight: // 1
			// Do this ?? maybe not??
			temp_additional_rotation_z = glm::radians(0.0f);
			temp_additional_rotation_y = glm::radians(0.0f);
			is_mouse_dragging = false;
			update_camera_position();
			break;
		case Can::MouseCode::Button2:
			break;
		case Can::MouseCode::Button3:
			break;
		case Can::MouseCode::Button4:
			break;
		case Can::MouseCode::Button5:
			break;
		case Can::MouseCode::Button6:
			break;
		case Can::MouseCode::Button7:
			break;
		default:
			break;
		}
		return false;
	}
	bool Perspective_Camera_Controller::on_mouse_released(Event::MouseButtonReleasedEvent& event)
	{
		MouseCode key_code = event.GetMouseButton();
		switch (key_code)
		{
		case Can::MouseCode::ButtonLeft: // 0
			// Apply rotation/ orbitting
			is_mouse_dragging = false;

			center_rot.z += temp_additional_rotation_z;
			center_rot.y += temp_additional_rotation_y;

			temp_additional_rotation_z = 0.0f;
			temp_additional_rotation_y = 0.0f;

			if (mode == Mode::GamePlay || mode == Mode::FollowThirdPerson) {
				camera.set_rotation(center_rot);
				update_camera_position();
			}
			break;
		case Can::MouseCode::ButtonRight: // 1
			break;
		case Can::MouseCode::Button2:
			break;
		case Can::MouseCode::Button3:
			break;
		case Can::MouseCode::Button4:
			break;
		case Can::MouseCode::Button5:
			break;
		case Can::MouseCode::Button6:
			break;
		case Can::MouseCode::Button7:
			break;
		default:
			break;
		}
		return false;
	}
}