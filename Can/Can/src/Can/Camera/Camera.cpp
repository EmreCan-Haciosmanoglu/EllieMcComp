#include "canpch.h"
#include "Camera.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Can/Instrumentor.h"

namespace Can::Camera
{
	Perspective::Perspective()
	{
		CAN_PROFILE_FUNCTION();

		cameraTarget = { 0.0f, 0.0f, 0.0f };
		cameraDirection = glm::normalize(m_Position - cameraTarget);
		up = v3{ 0.0f, 0.0f, 1.0f };
		cameraRight = glm::normalize(glm::cross(up, cameraDirection));
		cameraUp = glm::cross(cameraDirection, cameraRight);
		view = glm::lookAt(v3(0.0f, -2.0f, 3.0f),
			v3(0.0f, 0.0f, 0.0f),
			v3(0.0f, 0.0f, 1.0f));

	}
	void Perspective::Turn()
	{
		CAN_PROFILE_FUNCTION();

		f32 radius = 10.0f;
		f32 camX = (f32)sin(glfwGetTime()) * radius;
		f32 camZ = (f32)cos(glfwGetTime()) * radius;
		view = glm::lookAt(v3(camX, 0.0f, camZ), v3{ 0.0f, 0.0f, 0.0f }, v3{ 0.0f, 0.0f, 1.0f });
	}

	void Camera::RecalculateViewMatrix()
	{
		CAN_PROFILE_FUNCTION();

		glm::mat4 transform = glm::mat4(1.0f);
		transform = glm::rotate(transform, glm::radians(m_Rotation.x), v3(1.0f, 0.0f, 0.0f));
		transform = glm::rotate(transform, glm::radians(m_Rotation.y), v3(0.0f, 1.0f, 0.0f));
		transform = glm::rotate(transform, glm::radians(m_Rotation.z), v3(0.0f, 0.0f, 1.0f));
		transform = glm::translate(transform, m_Position);

		view = glm::inverse(transform);
		m_ViewProjectionMatrix = m_ProjectionMatrix * view;
	}

	OrthographicCamera::OrthographicCamera(f32 left, f32 right, f32 bottom, f32 top, f32 n, f32 f)
	{
		CAN_PROFILE_FUNCTION();

		SetProjection(left, right, bottom, top, n, f);
	}
	void OrthographicCamera::SetProjection(f32 left, f32 right, f32 bottom, f32 top, f32 n, f32 f)
	{
		CAN_PROFILE_FUNCTION();

		m_ProjectionMatrix = glm::ortho(left, right, bottom, top, n, f);
		m_ViewProjectionMatrix = m_ProjectionMatrix * view;
	}

	void OrthographicCamera::RecalculateViewMatrix()
	{
		CAN_PROFILE_FUNCTION();

		m4 transform =
			glm::translate(m4(1.0f), m_Position) *
			glm::rotate(m4(1.0f), m_Rotation.z, v3{ 0.0f, 0.0f, 1.0f }) *
			glm::rotate(m4(1.0f), m_Rotation.y, v3{ 0.0f, 1.0f, 0.0f }) *
			glm::rotate(m4(1.0f), m_Rotation.x, v3{ 1.0f, 0.0f, 0.0f });

		view = glm::inverse(transform);
		m_ViewProjectionMatrix = m_ProjectionMatrix * view;
	}

}