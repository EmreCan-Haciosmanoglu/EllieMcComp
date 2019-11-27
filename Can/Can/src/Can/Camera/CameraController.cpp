#include "canpch.h"
#include "CameraController.h"
#include "Can/Input.h"

namespace Can::Camera
{
	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
		: m_AspectRatio(aspectRatio)
		, m_Rotation(rotation)
		, m_Camera(-aspectRatio * m_ZoomLevel, aspectRatio* m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel)
	{
	}

	void OrthographicCameraController::OnUpdate(TimeStep ts)
	{
		if (Input::IsKeyPressed(CAN_KEY_A))
		{
			m_CameraPosition.x -= cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
			m_CameraPosition.y -= sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
		}
		else if (Input::IsKeyPressed(CAN_KEY_D))
		{
			m_CameraPosition.x += cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
			m_CameraPosition.y += sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
		}

		if (Input::IsKeyPressed(CAN_KEY_W))
		{
			m_CameraPosition.x += -sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
			m_CameraPosition.y += cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
		}
		else if (Input::IsKeyPressed(CAN_KEY_S))
		{
			m_CameraPosition.x -= -sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
			m_CameraPosition.y -= cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
		}

		if (m_Rotation)
		{
			if (Input::IsKeyPressed(CAN_KEY_Q))
				m_CameraRotation += m_CameraRotationSpeed * ts;
			else if (Input::IsKeyPressed(CAN_KEY_E))
				m_CameraRotation -= m_CameraRotationSpeed * ts;

			if (m_CameraRotation > 180.0f)
				m_CameraRotation -= 360.0f;
			else if (m_CameraRotation <= -180.0f)
				m_CameraRotation += 360.0f;

			m_Camera.SetRotation({ m_Camera.GetRotation().x, m_Camera.GetRotation().y, m_CameraRotation });
		}

		m_Camera.SetPosition(m_CameraPosition);
	}

	void OrthographicCameraController::OnEvent(Event::Event & e)
	{
		Event::EventDispatcher dispatcher(e);
		dispatcher.Dispatch< Event::MouseScrolledEvent>(CAN_BIND_EVENT_FN(Camera::OrthographicCameraController::OnMouseScrollEvent));
		dispatcher.Dispatch< Event::WindowResizeEvent>(CAN_BIND_EVENT_FN(Camera::OrthographicCameraController::OnWindowResized));
	}

	bool OrthographicCameraController::OnMouseScrollEvent(Event::MouseScrolledEvent & e)
	{
		m_ZoomLevel -= e.GetYOffset();
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);

		return false;
	}

	bool OrthographicCameraController::OnWindowResized(Event::WindowResizeEvent & e)
	{
		m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);

		return false;
	}

}

namespace Can::Camera::Controller
{
	Perspective::Perspective(float fovy, float aspectRatio, float n, float f)
		: m_Fovy(fovy)
		, m_AspectRatio(aspectRatio)
		, m_Camera(m_Fovy, m_AspectRatio,n,f)
	{
	}

	void Perspective::OnUpdate(Can::TimeStep ts)
	{
		float rotx = glm::radians(m_CameraRotation.x);
		float roty = glm::radians(m_CameraRotation.y);
		float rotz = glm::radians(m_CameraRotation.z);
		float tSpeed = m_CameraTranslationSpeed * ts;
		float rSpeed = m_CameraRotationSpeed * ts;

		if (Input::IsKeyPressed(CAN_KEY_D))
			m_CameraPosition.x -= tSpeed;
		else if (Input::IsKeyPressed(CAN_KEY_A))
			m_CameraPosition.x += tSpeed;

		if (Input::IsKeyPressed(CAN_KEY_W))
			m_CameraPosition.z -= tSpeed;
		else if (Input::IsKeyPressed(CAN_KEY_S))
			m_CameraPosition.z += tSpeed;

		if (Input::IsKeyPressed(CAN_KEY_SPACE))
			m_CameraPosition.y -= tSpeed;
		else if(Input::IsKeyPressed(CAN_KEY_LEFT_SHIFT))
			m_CameraPosition.y += tSpeed;


		m_Camera.SetPosition(m_CameraPosition);

		if (Input::IsKeyPressed(CAN_KEY_G))
			m_CameraRotation.y += m_CameraRotationSpeed * ts;
		else if (Input::IsKeyPressed(CAN_KEY_J))
			m_CameraRotation.y -= m_CameraRotationSpeed * ts;

		if (Input::IsKeyPressed(CAN_KEY_Y))
			m_CameraRotation.x += m_CameraRotationSpeed * ts;
		else if (Input::IsKeyPressed(CAN_KEY_H))
			m_CameraRotation.x -= m_CameraRotationSpeed * ts;

		if (m_CameraRotation.x > 180.0f)
			m_CameraRotation.x -= 360.0f;
		else if (m_CameraRotation.x <= -180.0f)
			m_CameraRotation.x += 360.0f;

		if (m_CameraRotation.y > 180.0f)
			m_CameraRotation.y -= 360.0f;
		else if (m_CameraRotation.y <= -180.0f)
			m_CameraRotation.y += 360.0f;

		if (Input::IsKeyPressed(CAN_KEY_KP_ADD))
			m_Fovy += 2.0f * ts;
		else if (Input::IsKeyPressed(CAN_KEY_KP_ENTER))
			m_Fovy -= 2.0f * ts;

		m_Camera.SetRotation(m_CameraRotation);
		m_Camera.SetProjection(m_Fovy);

	}

	void Perspective::OnEvent(Can::Event::Event & e)
	{
		Event::EventDispatcher dispatcher(e);
		dispatcher.Dispatch< Event::WindowResizeEvent>(CAN_BIND_EVENT_FN(Camera::Controller::Perspective::OnWindowResized));
	}

	bool Perspective::OnWindowResized(Event::WindowResizeEvent & e)
	{
		if (e.GetWidth() == 0 || e.GetHeight() == 0)
			return false;
		m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
		m_Camera.SetProjection(m_Fovy, m_AspectRatio);

		return false;
	}
}
