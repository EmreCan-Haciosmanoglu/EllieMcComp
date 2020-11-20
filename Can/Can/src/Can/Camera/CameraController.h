#pragma once
#include "Can/Camera/Camera.h"
#include "Can/Core/TimeStep.h"

#include "Can/Events/ApplicationEvent.h"
#include "Can/Events/MouseEvent.h"
#include "Can/Events/KeyEvent.h"


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
		OrthographicCameraController(float aspectRatio, float zoomLevel, bool rotation = false);

		void OnUpdate(Can::TimeStep ts);
		void OnEvent(Can::Event::Event& e);

		OrthographicCamera& GetCamera() { return m_Camera; }
		const OrthographicCamera& GetCamera() const { return m_Camera; }
	private:
		bool OnMouseScrollEvent(Event::MouseScrolledEvent& e);
		bool OnWindowResized(Event::WindowResizeEvent& e);

	private:
		float m_ZoomLevel = 10.0f;
		float m_AspectRatio;
		OrthographicCamera m_Camera;

		bool m_Rotation;

		glm::vec3 m_CameraPosition = { 0.0f,0.0f,0.0f };
		float m_CameraRotation = 0.0f;

		float m_CameraTranslationSpeed = 5.0f;
		float m_CameraRotationSpeed = 90.0f;
	};
}

namespace Can::Camera::Controller
{
	class Perspective : public CameraController
	{
	public:
		Perspective(
			float
			fovy,
			float aspectRatio,
			float n, float f,
			const glm::vec3& pos = { 0,0,0 },
			const glm::vec3& rot = { 0,0,0 }
		);

		void OnUpdate(Can::TimeStep ts);
		void OnEvent(Can::Event::Event& e);

		PerspectiveCamera& GetCamera() { return m_Camera; }
		const PerspectiveCamera& GetCamera() const { return m_Camera; }

		inline float GetAspectRatio() { return m_AspectRatio; }
		inline const float GetAspectRatio() const { return m_AspectRatio; }

		inline float GetFOV() { return m_Fovy; }
		inline const float GetFOV() const { return m_Fovy; }

		inline float GetNear() { return m_Near; }
		inline const float GetNear() const { return m_Near; }

		inline float GetFar() { return m_Far; }
		inline const float GetFar() const{ return m_Far; }

	private:
		bool OnWindowResized(Event::WindowResizeEvent& e);
	private:
		float m_AspectRatio;
		float m_Fovy;
		float m_Near;
		float m_Far;

		PerspectiveCamera m_Camera;

		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_CameraRotation = { 0.0f, 0.0f, 0.0f };

		float m_CameraTranslationSpeed = 5.0f;
		float m_CameraRotationSpeed = 90.0f;
	};
}