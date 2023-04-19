#pragma once
#include "Can/Camera/Perspective_Camera.h"
#include "Can/Core/TimeStep.h"

#include "Can/Events/ApplicationEvent.h"
#include "Can/Events/MouseEvent.h"
#include "Can/Events/KeyEvent.h"

namespace Can
{
	class Object;
	class Perspective_Camera_Controller
	{
	public:
		Perspective_Camera_Controller(
			f32 fovy, f32 aspect, f32 n, f32 f, v3 pos, v3 rot);

		void on_update(Can::TimeStep ts);
		void on_event(Event::Event& e);

		void translate(v3 direction, f32 length);
		void translate_relative(v3 direction, f32 length);
		void pitch(f32 amount_in_radians, bool clockwise = false);
		void orbit_around_point(f32 amount_in_radians, v3 point, bool clockwise = false);

	private:
		bool on_window_resized(Event::WindowResizeEvent& event);

		bool on_mouse_moved(Event::MouseMovedEvent& event);
		bool on_mouse_scrolled(Event::MouseScrolledEvent& event);
		bool on_mouse_pressed(Event::MouseButtonPressedEvent& event);
		bool on_mouse_released(Event::MouseButtonReleasedEvent& event);

		bool on_key_released(Event::KeyReleasedEvent& event);

		void update_camera_position();

	public:

		enum class Mode : u8 {
			FreeMoving,
			GamePlay,
			FollowFirstPerson,
			FollowThirdPerson
		};

		Perspective_Camera camera;

		KeyCode forward_key = KeyCode::W;
		KeyCode backward_key = KeyCode::S;
		KeyCode left_key = KeyCode::A;
		KeyCode right_key = KeyCode::D;
		KeyCode lower_key = KeyCode::LeftShift;
		KeyCode raise_key = KeyCode::Space;
		KeyCode rotate_cw_key = KeyCode::Q;
		KeyCode rotate_ccw_key = KeyCode::E;
		KeyCode pitch_down_key = KeyCode::Y;
		KeyCode pitch_up_key = KeyCode::H;
		KeyCode increase_fov_key = KeyCode::KPAdd;
		KeyCode decrease_fov_key = KeyCode::KPEnter;
		KeyCode mode_cycle_key = KeyCode::KP0;

		v3 center_pos{ 0.0f, 0.0f, 0.0f };
		v3 center_rot{ 0.0f, 0.0f, 0.0f };
		
		f32 min_pos_z = 0.5f;
		f32 max_pos_z = 55.0f;
		f32 min_rot_y = -90.0f;
		f32 max_rot_y = -30.0f;
		f32 zoom_t = 0.6f;

		f32 translation_speed = 5.0f;
		f32 rotation_speed = 60.0f;
		f32 rotation_with_mouse_speed_multiplier_z = 0.5f;
		f32 rotation_with_mouse_speed_multiplier_y = 0.25f;
		bool is_y_inverted = false;
		f32 zoom_speed = 0.005f;

		Mode mode = Mode::GamePlay;
		Object* follow_object = nullptr;


		v2 mouse_drag_start_pos{};
		bool is_mouse_dragging = false;
		f32 temp_additional_rotation_z = 0.0f;
		f32 temp_additional_rotation_y = 0.0f;
	};
}