#include "canpch.h"
#include "CameraController.h"
#include "Can/Input.h"
#include "../../../../Game/src/Helper.h"


namespace Can::Camera
{
	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
		: m_AspectRatio(aspectRatio)
		, m_Rotation(rotation)
		, m_Camera(-aspectRatio * m_ZoomLevel, aspectRatio* m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel)
	{
	}
	OrthographicCameraController::OrthographicCameraController(float aspectRatio, float zoomLevel, bool rotation)
		: m_AspectRatio(aspectRatio)
		, m_ZoomLevel(zoomLevel)
		, m_Rotation(rotation)
		, m_Camera(-aspectRatio * m_ZoomLevel, aspectRatio* m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel)
	{
	}

	void OrthographicCameraController::OnUpdate(TimeStep ts)
	{
		CAN_PROFILE_FUNCTION();

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

	void OrthographicCameraController::OnEvent(Event::Event& e)
	{
		CAN_PROFILE_FUNCTION();

		Event::EventDispatcher dispatcher(e);
		dispatcher.Dispatch< Event::MouseScrolledEvent>(CAN_BIND_EVENT_FN(Camera::OrthographicCameraController::OnMouseScrollEvent));
		dispatcher.Dispatch< Event::WindowResizeEvent>(CAN_BIND_EVENT_FN(Camera::OrthographicCameraController::OnWindowResized));
	}

	bool OrthographicCameraController::OnMouseScrollEvent(Event::MouseScrolledEvent& e)
	{
		CAN_PROFILE_FUNCTION();

		m_ZoomLevel -= e.GetYOffset();
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);

		return false;
	}

	bool OrthographicCameraController::OnWindowResized(Event::WindowResizeEvent& e)
	{
		CAN_PROFILE_FUNCTION();

		m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);

		return false;
	}

}

namespace Can::Camera::Controller
{
	Perspective::Perspective(
		float fovy,
		float aspectRatio,
		float n, float f,
		const glm::vec3& pos,
		const glm::vec3& rot
	)
		: m_AspectRatio(aspectRatio)
		, m_Fovy(fovy)
		, m_Near(n)
		, m_Far(f)
		, m_CameraPosition(pos)
		, m_CameraRotation(rot)
		, m_Camera(m_Fovy, m_AspectRatio, m_Near, m_Far)
	{
		CAN_PROFILE_FUNCTION();

		m_Camera.SetPosition(pos);
		m_Camera.SetRotation(rot);
	}

	void Perspective::OnUpdate(Can::TimeStep ts)
	{
		CAN_PROFILE_FUNCTION();

		float rotx = glm::radians(m_CameraRotation.x);
		float roty = glm::radians(m_CameraRotation.y);
		float tSpeed = m_CameraTranslationSpeed * ts;
		float rSpeed = m_CameraRotationSpeed * ts;

		if (Input::IsKeyPressed(CAN_KEY_D))
		{
			m_CameraPosition.x += cos(roty) * tSpeed;
			m_CameraPosition.z -= sin(roty) * tSpeed;
		}
		else if (Input::IsKeyPressed(CAN_KEY_A))
		{
			m_CameraPosition.x -= cos(roty) * tSpeed;
			m_CameraPosition.z += sin(roty) * tSpeed;
		}

		if (Input::IsKeyPressed(CAN_KEY_W))
		{
			m_CameraPosition.x -= sin(roty) * tSpeed;
			m_CameraPosition.z -= cos(roty) * tSpeed;
		}
		else if (Input::IsKeyPressed(CAN_KEY_S))
		{
			m_CameraPosition.x += sin(roty) * tSpeed;
			m_CameraPosition.z += cos(roty) * tSpeed;
		}

		if (Input::IsKeyPressed(CAN_KEY_Q))
		{
			float angle = glm::radians(rSpeed);
			glm::vec3 direction = {
					-glm::sin(glm::radians(m_CameraRotation.y)) * glm::cos(glm::radians(m_CameraRotation.x)),
					glm::sin(glm::radians(m_CameraRotation.x)),
					-glm::cos(glm::radians(m_CameraRotation.x)) * glm::cos(glm::radians(m_CameraRotation.y))
			};
			glm::vec3 center = Helper::RayPlaneIntersection(m_CameraPosition, direction, { 0,0,0 }, { 0,1,0 });
			float rotatedX = glm::cos(angle) * (m_CameraPosition.x - center.x) - glm::sin(angle) * (m_CameraPosition.z - center.z) + center.x;

			float rotatedZ = glm::sin(angle) * (m_CameraPosition.x - center.x) + glm::cos(angle) * (m_CameraPosition.z - center.z) + center.z;
			m_CameraRotation.y -= rSpeed;
			m_CameraPosition = { rotatedX, m_CameraPosition.y, rotatedZ };
		}
		else if (Input::IsKeyPressed(CAN_KEY_E))
		{
			float angle = glm::radians(-rSpeed);
			glm::vec3 direction = {
					-glm::sin(glm::radians(m_CameraRotation.y)) * glm::cos(glm::radians(m_CameraRotation.x)),
					glm::sin(glm::radians(m_CameraRotation.x)),
					-glm::cos(glm::radians(m_CameraRotation.x)) * glm::cos(glm::radians(m_CameraRotation.y))
			};
			glm::vec3 center = Helper::RayPlaneIntersection(m_CameraPosition, direction, { 0,0,0 }, { 0,1,0 });
			float rotatedX = glm::cos(angle) * (m_CameraPosition.x - center.x) - glm::sin(angle) * (m_CameraPosition.z - center.z) + center.x;

			float rotatedZ = glm::sin(angle) * (m_CameraPosition.x - center.x) + glm::cos(angle) * (m_CameraPosition.z - center.z) + center.z;
			m_CameraRotation.y += rSpeed;
			m_CameraPosition = { rotatedX, m_CameraPosition.y, rotatedZ };
		}

		if (Input::IsKeyPressed(CAN_KEY_SPACE))
		{
			m_CameraPosition.y -= tSpeed;
		}
		else if (Input::IsKeyPressed(CAN_KEY_LEFT_SHIFT))
		{
			m_CameraPosition.y += tSpeed;
		}


		if (Input::IsKeyPressed(CAN_KEY_Y))
			m_CameraRotation.x += rSpeed;
		else if (Input::IsKeyPressed(CAN_KEY_H))
			m_CameraRotation.x -= rSpeed;

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

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);
		m_Camera.SetProjection(m_Fovy, m_AspectRatio, m_Near, m_Far);

	}

	void Perspective::OnEvent(Can::Event::Event& e)
	{
		CAN_PROFILE_FUNCTION();

		Event::EventDispatcher dispatcher(e);
		dispatcher.Dispatch< Event::WindowResizeEvent>(CAN_BIND_EVENT_FN(Camera::Controller::Perspective::OnWindowResized));
	}

	bool Perspective::OnWindowResized(Event::WindowResizeEvent& e)
	{
		CAN_PROFILE_FUNCTION();

		if (e.GetWidth() == 0 || e.GetHeight() == 0)
			return false;
		m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
		m_Camera.SetProjection(m_Fovy, m_AspectRatio);

		return false;
	}
}
