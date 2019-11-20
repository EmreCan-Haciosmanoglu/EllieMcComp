#include "canpch.h"
#include "Renderer.h"
#include "Renderer2D.h"
#include "Renderer3D.h"

#include "Platform/OpenGl/OpenGLShader.h"

namespace Can
{
	Scope<Renderer::SceneData> Renderer::s_SceneData = CreateScope<Renderer::SceneData>();

	void Renderer::Init()
	{
		RenderCommand::Init();
		Renderer2D::Init();
		Renderer3D::Init();
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}

	void Renderer::BeginScene(Camera::Camera& camera)
	{
		s_SceneData->ViewProjectMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(
		const Ref<Shader>& shader,
		const Ref<VertexArray>& vertexArray,
		const glm::mat4& transform
	)
	{
		shader->Bind();
		Ref<OpenGLShader> openglshader = std::dynamic_pointer_cast<OpenGLShader>(shader);
		if (openglshader)
		{
			openglshader->UploadUniformMat4("u_ViewProjection", s_SceneData->ViewProjectMatrix);
			openglshader->UploadUniformMat4("u_Transform", transform);
		}

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
}