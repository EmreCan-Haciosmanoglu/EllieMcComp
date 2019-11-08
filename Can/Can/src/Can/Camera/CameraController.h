#pragma once
#include "Can/Camera/Camera.h"
#include "Can/Core/TimeStep.h"

#include "Can/Events/ApplicationEvent.h"
#include "Can/Events/MouseEvent.h"


namespace Can::Camera
{
	class CameraController
	{
	public:

	private:

	};

	class OrthographicCameraController : public CameraController
	{
	public:
		OrthographicCameraController(float aspectRatio, bool rotation = false);

		void OnUpdate(Can::TimeStep ts);
		void OnEvent(Can::Event::Event& e);

		OrthographicCamera& GetCamera() { return m_Camera; }
		const OrthographicCamera& GetCamera() const { return m_Camera; }
	private:
		bool OnMouseScrollEvent(Can::Event::MouseScrolledEvent& e);
		bool OnWindowResized(Can::Event::WindowResizeEvent& e);

	private:
		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;
		OrthographicCamera m_Camera;

		bool m_Rotation;

		glm::vec3 m_CameraPosition = { 0.0f,0.0f,0.0f };
		float m_CameraRotation = 0.0f;

		float m_CameraTranslationSpeed = 5.0f;
		float m_CameraRotationSpeed = 90.0f;
	};
}