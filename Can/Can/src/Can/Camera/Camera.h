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
		const v3& GetPosition() const { return m_Position; }
		void SetPosition(const v3& position) { m_Position = position; RecalculateViewMatrix(); }

		const v3& GetRotation() const { return m_Rotation; }
		void SetRotation(const v3& rotation) { m_Rotation = rotation; RecalculateViewMatrix(); }

		const glm::mat4& GetViewMatrix() const { return view; }
		const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

	protected:
		v3 m_Position{ 0.0f, 0.0f, 0.0f };
		v3 m_Rotation{ 0.0f, 0.0f, 0.0f };
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
		v3 cameraTarget;
		v3 cameraDirection;
		v3 up;
		v3 cameraRight;
		v3 cameraUp;
	};

	class OrthographicCamera : public Camera
	{
	public:
		OrthographicCamera(f32 left, f32 right, f32 bottom, f32 top, f32 n = -1.0f, f32 f = 1.0f);
		void SetProjection(f32 left, f32 right, f32 bottom, f32 top, f32 n = -1.0f, f32 f = 1.0f);

		virtual ~OrthographicCamera() {}

		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
	private:
		void RecalculateViewMatrix();

	private:

		v3 m_Position{ 0.0f, 0.0f, 0.0f };
		v3 m_Rotation{ 0.0f, 0.0f, 0.0f };
	};

}