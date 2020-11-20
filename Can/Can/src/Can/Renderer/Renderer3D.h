#pragma once
#include "Can/Camera/Camera.h"
#include "Object.h"

namespace Can
{
	class Renderer3D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const Camera::PerspectiveCamera& camera);
		static void EndScene();

		static void AddObject(Object* obj);
		static void DeleteObject(Object* obj);


		static void DrawObjects(const glm::mat4& lightspace, unsigned int shadowMapTexture, const Camera::PerspectiveCamera& camera, const glm::vec3& lightPos);

	public:
		static std::vector<Object*> s_Objects;
	};
}