#include "canpch.h"
#include "Orthographic_Camera_Controller.h"
#include "Can/Input.h"

#include "Can/Math.h"


namespace Can
{
	Orthographic_Camera_Controller::Orthographic_Camera_Controller()
		:m_Camera(-10.0f, 10.0f, -10.0f, 10.0f) // @DeleteMe
	{

	}

	void Orthographic_Camera_Controller::on_update(TimeStep ts)
	{
		CAN_PROFILE_FUNCTION();

		if (Input::IsKeyPressed(KeyCode::A))
		{
			m_CameraPosition.x -= cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
			m_CameraPosition.y -= sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
		}
		else if (Input::IsKeyPressed(KeyCode::D))
		{
			m_CameraPosition.x += cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
			m_CameraPosition.y += sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
		}

		if (Input::IsKeyPressed(KeyCode::W))
		{
			m_CameraPosition.x += -sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
			m_CameraPosition.y += cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
		}
		else if (Input::IsKeyPressed(KeyCode::S))
		{
			m_CameraPosition.x -= -sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
			m_CameraPosition.y -= cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
		}

		if (m_Rotation)
		{
			if (Input::IsKeyPressed(KeyCode::Q))
				m_CameraRotation += m_CameraRotationSpeed * ts;
			else if (Input::IsKeyPressed(KeyCode::E))
				m_CameraRotation -= m_CameraRotationSpeed * ts;

			if (m_CameraRotation > 180.0f)
				m_CameraRotation -= 360.0f;
			else if (m_CameraRotation <= -180.0f)
				m_CameraRotation += 360.0f;

			m_Camera.SetRotation({ m_Camera.GetRotation().x, m_Camera.GetRotation().y, m_CameraRotation });
		}

		m_Camera.SetPosition(m_CameraPosition);
	}

	void Orthographic_Camera_Controller::on_event(Event::Event& e)
	{
		CAN_PROFILE_FUNCTION();

		Event::EventDispatcher dispatcher(e);
		dispatcher.Dispatch< Event::MouseScrolledEvent>(CAN_BIND_EVENT_FN(Orthographic_Camera_Controller::on_mouse_scroll_event));
		dispatcher.Dispatch< Event::WindowResizeEvent>(CAN_BIND_EVENT_FN(Orthographic_Camera_Controller::on_window_resized));
	}

	bool Orthographic_Camera_Controller::on_mouse_scroll_event(Event::MouseScrolledEvent& e)
	{
		CAN_PROFILE_FUNCTION();

		m_ZoomLevel -= e.GetYOffset();
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);

		return false;
	}

	bool Orthographic_Camera_Controller::on_window_resized(Event::WindowResizeEvent& e)
	{
		CAN_PROFILE_FUNCTION();
		if (e.width == 0 || e.height == 0)
			return false;
		m_AspectRatio = (f32)e.width / (f32)e.width;
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);

		return false;
	}

	void init_orthographic_camera_controller(Orthographic_Camera_Controller& camera_controller, f32 aspect_ratio, f32 zoom_level, bool rotation)
	{
		camera_controller.m_AspectRatio = aspect_ratio;
		camera_controller.m_ZoomLevel = zoom_level;
		camera_controller.m_Rotation = rotation;
		camera_controller.m_Camera = Camera::OrthographicCamera(-aspect_ratio * zoom_level, aspect_ratio * zoom_level, -zoom_level, zoom_level, -100.0f, 100.0f);
	}

}
