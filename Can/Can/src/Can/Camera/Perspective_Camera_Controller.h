#pragma once
#include "Can/Camera/Perspective_Camera.h"
#include "Can/Core/TimeStep.h"

#include "Can/Events/ApplicationEvent.h"
#include "Can/Events/MouseEvent.h"
#include "Can/Events/KeyEvent.h"

namespace Can
{
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
		bool on_mouse_pressed(Event::MouseButtonPressedEvent& event);
		bool on_mouse_scrolled(Event::MouseScrolledEvent& event);
		void update_camera_position();

	public:
		Perspective_Camera camera;

		KeyCode forward_key = KeyCode::W;
		KeyCode backward_key = KeyCode::S;
		KeyCode left_key = KeyCode::A;
		KeyCode right_key = KeyCode::D;
		KeyCode rotate_cw_key = KeyCode::Q;
		KeyCode rotate_ccw_key = KeyCode::E;
		KeyCode pitch_down_key = KeyCode::Y;
		KeyCode pitch_up_key = KeyCode::H;
		KeyCode increase_fov_key = KeyCode::KPAdd;
		KeyCode decrease_fov_key = KeyCode::KPEnter;
		MouseCode zoom_in_key = MouseCode::Button3;
		MouseCode zoom_out_key = MouseCode::Button4;

		v3 center{ 0.0f, 0.0f, 0.0f };
		f32 min_pos_z = 0.5f;
		f32 max_pos_z = 55.0f;
		f32 min_rot_y = 30.0f;
		f32 max_rot_y = 90.0f;
		f32 zoom_t = 0.6f;

	private:
		f32 translation_speed = 5.0f;
		f32 rotation_speed = 60.0f;
		f32 zoom_speed = 0.005f;
	};
}