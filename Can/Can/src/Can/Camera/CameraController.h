#pragma once
#include "Can/Camera/Camera.h"
#include "Can/Core/TimeStep.h"

#include "Can/Events/ApplicationEvent.h"
#include "Can/Events/MouseEvent.h"
#include "Can/Events/KeyEvent.h"


namespace Can::Camera
{
	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(f32 aspectRatio, bool rotation = false);
		OrthographicCameraController(f32 aspectRatio, f32 zoomLevel, bool rotation = false);

		void OnUpdate(Can::TimeStep ts);
		void OnEvent(Can::Event::Event& e);

		OrthographicCamera& GetCamera() { return m_Camera; }
		const OrthographicCamera& GetCamera() const { return m_Camera; }
	private:
		bool OnMouseScrollEvent(Event::MouseScrolledEvent& e);
		bool OnWindowResized(Event::WindowResizeEvent& e);

	private:
		f32 m_ZoomLevel = 10.0f;
		f32 m_AspectRatio;
		OrthographicCamera m_Camera;

		bool m_Rotation;

		v3 m_CameraPosition = { 0.0f,0.0f,0.0f };
		f32 m_CameraRotation = 0.0f;

		f32 m_CameraTranslationSpeed = 5.0f;
		f32 m_CameraRotationSpeed = 30.0f;
	};
}
