#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


namespace Can::Camera
{
	class Camera
	{
	protected:
		Camera() {}
		virtual ~Camera() {}
	public:
		const glm::vec3& GetPosition() const { return m_Position; }
		void SetPosition(const glm::vec3& position) { m_Position = position; RecalculateViewMatrix(); }

		const glm::vec3& GetRotation() const { return m_Rotation; }
		void SetRotation(const glm::vec3& rotation) { m_Rotation = rotation; RecalculateViewMatrix(); }

		const glm::mat4& GetViewMatrix() const { return view; }
		const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }
	
	protected:
		glm::vec3 m_Position = { 0.0f,0.0f,0.0f };
		glm::vec3 m_Rotation = { 0.0f,0.0f,0.0f };
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 m_ViewProjectionMatrix;
		glm::mat4 m_ProjectionMatrix;

	private:
		void RecalculateViewMatrix();
	};

	class Perspective : public Camera
	{
	public:
		Perspective();
		virtual ~Perspective() override {}

		void Turn();

	private:
		glm::vec3 cameraTarget;
		glm::vec3 cameraDirection;
		glm::vec3 up;
		glm::vec3 cameraRight;
		glm::vec3 cameraUp;
	};

	class OrthographicCamera : public Camera
	{
	public:
		OrthographicCamera(float left, float right, float bottom, float top, float n = -1.0f, float f = 1.0f);
		void SetProjection(float left, float right, float bottom, float top, float n = -1.0f, float f = 1.0f);

		virtual ~OrthographicCamera() {}

		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
	private:
		void RecalculateViewMatrix();

	private:

		glm::vec3 m_Position = { 0.0f,0.0f,0.0f };
		glm::vec3 m_Rotation = { 0.0f,0.0f,0.0f };
	};

	class PerspectiveCamera
	{
	public:
		PerspectiveCamera(float fovy, float aspect, float n = 0.1f, float f = 10.0f); 
		void SetProjection(float fovy, float aspect, float n = 0.1f, float f = 10.0f);
		void SetProjection(float fovy);

		const glm::vec3& GetPosition() const { return m_Position; }
		void SetPosition(const glm::vec3& position) { m_Position = position; RecalculateViewMatrix(); }

		const glm::vec3& GetRotation() const { return m_Rotation; }
		void SetRotation(const glm::vec3& rotation) { m_Rotation = rotation; RecalculateViewMatrix(); }

		const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

	private:
		void RecalculateViewMatrix();

	private:
		glm::vec3 m_Position = { 0.0f,0.0f,0.0f };
		glm::vec3 m_Rotation = { 0.0f,0.0f,0.0f };
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewProjectionMatrix;
	};
}