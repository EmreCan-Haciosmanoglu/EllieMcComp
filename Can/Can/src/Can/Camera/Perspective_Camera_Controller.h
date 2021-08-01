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

	public:
		Perspective_Camera camera;

	private:
		f32 translation_speed = 5.0f;
		f32 rotation_speed = 60.0f;
	};
}