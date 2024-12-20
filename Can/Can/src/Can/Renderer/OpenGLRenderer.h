#pragma once

#include "RenderCommand.h"
#include "Can/Camera/Camera.h"
#include "Shader.h"

namespace Can
{
	class OpenGLRenderer
	{
	public:
		static void Init();
		static void Shutdown();

		static void OnWindowResize(uint32_t width, uint32_t height);

		static void BeginScene(Camera::Camera& camera);
		static void EndScene();

		static void Submit(
			const Ref<Shader>& shader,
			const Ref<VertexArray>& vertexArray,
			const glm::mat4& transform = glm::mat4(1.0f)
		);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	private:
		struct SceneData
		{
			glm::mat4 ViewProjectMatrix;
		};

		static Scope<SceneData> s_SceneData;
	};
}