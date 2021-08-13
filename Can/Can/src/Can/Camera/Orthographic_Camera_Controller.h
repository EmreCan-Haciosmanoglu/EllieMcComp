#pragma once
#include "Can/Camera/Camera.h"
#include "Can/Core/TimeStep.h"

#include "Can/Events/ApplicationEvent.h"
#include "Can/Events/MouseEvent.h"
#include "Can/Events/KeyEvent.h"


namespace Can
{
	class Orthographic_Camera_Controller
	{
	public:
		Orthographic_Camera_Controller();

		void on_update(Can::TimeStep ts);
		void on_event(Can::Event::Event& e);

	
		bool on_mouse_scroll_event(Event::MouseScrolledEvent& e);
		bool on_window_resized(Event::WindowResizeEvent& e);

	
		f32 m_ZoomLevel = 0.0f;
		f32 m_AspectRatio = 0.0f;
		Camera::OrthographicCamera m_Camera;

		bool m_Rotation = false;

		v3 m_CameraPosition{ 0.0f, 0.0f, 0.0f };
		f32 m_CameraRotation = 0.0f;

		f32 m_CameraTranslationSpeed = 5.0f;
		f32 m_CameraRotationSpeed = 30.0f;
	};

	void init_orthographic_camera_controller(Orthographic_Camera_Controller& camera_controller, f32 aspect_ratio, f32 zoom_level, bool rotation);
}
