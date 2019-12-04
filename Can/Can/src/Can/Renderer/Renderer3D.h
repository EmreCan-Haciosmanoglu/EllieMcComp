#pragma once
#include "Can/Camera/Camera.h"
#include "Texture.h"
#include "VertexArray.h"
#include "Shader.h"

namespace Can
{
	class Renderer3D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const Camera::PerspectiveCamera& camera);
		static void EndScene();

		static void UpdateSphere(int resolution);

		static void DrawCube(const glm::vec3& position, const glm::vec3& scale);
		static void DrawCube(const glm::vec3& position, const glm::vec3& scale, const Ref<Texture2D>& texture);
	};
}