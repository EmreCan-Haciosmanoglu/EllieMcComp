#pragma once
#include "Can/Camera/Camera.h"
#include "Object.h"

namespace Can
{
	struct OutputTest
	{
		glm::mat4 lightSpaceMatrix;
		unsigned int depthMap;
		unsigned int depthMapFBO;
	};

	class Renderer3D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const Camera::PerspectiveCamera& camera);
		static void EndScene();

		static void AddObject(Object* obj);
		static void DeleteObject(Object* obj);

		static OutputTest Test(const Camera::OrthographicCamera& camera, const glm::vec3& lightPos);

		static void DrawObjects(const OutputTest& outputTest, const Camera::PerspectiveCamera& camera, const glm::vec3& lightPos);
		static void DrawObjectsForShadowMap(const Ref<Shader> shadowShader);
	};
}