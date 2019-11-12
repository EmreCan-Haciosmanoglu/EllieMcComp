#include "canpch.h"
#include "Camera.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Can::Camera
{
	Perspective::Perspective()
	{
		cameraTarget = { 0.0f, 0.0f, 0.0f };
		cameraDirection = glm::normalize(m_Position - cameraTarget);
		up = { 0.0f, 1.0f, 0.0f };
		cameraRight = glm::normalize(glm::cross(up, cameraDirection));
		cameraUp = glm::cross(cameraDirection, cameraRight);
		view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f),
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f));

	}
	void Perspective::Turn()
	{
		float radius = 10.0f;
		float camX = sin(glfwGetTime()) * radius;
		float camZ = cos(glfwGetTime()) * radius;
		view = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
	}
	void Camera::RecalculateViewMatrix()
	{
		glm::mat4 transform = glm::mat4(1.0f);
		transform = glm::translate(transform, m_Position);
		transform = glm::rotate(transform, glm::radians(m_Rotation.z), glm::vec3(0, 0, 1));
		transform = glm::rotate(transform, glm::radians(m_Rotation.y), glm::vec3(0, 1, 0));
		transform = glm::rotate(transform, glm::radians(m_Rotation.x), glm::vec3(1, 0, 0));

		view = glm::inverse(transform);
		m_ViewProjectionMatrix = m_ProjectionMatrix * view;
	}

	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top, float n, float f)	
	{
		SetProjection( left,  right,  bottom,  top,  n,  f);
	}
	void OrthographicCamera::SetProjection(float left, float right, float bottom, float top, float n, float f)
	{
		m_ProjectionMatrix = glm::ortho(left, right, bottom, top, n, f);
		m_ViewProjectionMatrix = m_ProjectionMatrix * view;
	}

	void OrthographicCamera::RecalculateViewMatrix()
	{
		glm::mat4 transform = glm::mat4(1.0f);
		transform = glm::translate(transform, m_Position);
		transform = glm::rotate(transform, glm::radians(m_Rotation.z), glm::vec3(0, 0, 1));
		transform = glm::rotate(transform, glm::radians(m_Rotation.y), glm::vec3(0, 1, 0));
		transform = glm::rotate(transform, glm::radians(m_Rotation.x), glm::vec3(1, 0, 0));

		view = glm::inverse(transform);
		m_ViewProjectionMatrix = m_ProjectionMatrix * view;
	}
}