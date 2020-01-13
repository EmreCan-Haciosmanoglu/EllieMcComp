#pragma once
#include "Can/Camera/Camera.h"
#include "Texture.h"
#include "VertexArray.h"
#include "Shader.h"

namespace Can
{
	struct Object
	{
		bool isEnabled = true;
		Ref<VertexBuffer> VB;
		Ref<IndexBuffer> IB;
		Ref<VertexArray> VA;
		Ref<Texture2D> T;
		Ref<Shader> S;
		float* Vertices;
		uint32_t* Indices;
		glm::mat4 transform;
		glm::vec3 position;
		glm::vec3 scale;
	};

	class Renderer3D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const Camera::PerspectiveCamera& camera);
		static void EndScene();
		
		static void AddObject(Object* obj);

		static void DrawObjects();
	};
}