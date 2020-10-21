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

		static void Test(const Camera::OrthographicCamera& camera);

		static void AddObject(Object* obj);
		static void DeleteObject(Object* obj);

		static void DrawObjects();
		static void DrawObjectsForShadowMap(const Ref<Shader> shadowShader);
	};
}