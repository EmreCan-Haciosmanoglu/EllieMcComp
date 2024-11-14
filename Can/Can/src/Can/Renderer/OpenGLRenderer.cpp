#include "canpch.h"
#include "OpenGLRenderer.h"
#include "Renderer2D.h"
#include "Renderer3D.h"

#include "Can/Immediate_Renderer/Immediate_Renderer.h"

#include "Platform/OpenGl/OpenGLShader.h"


namespace Can
{
	Scope<OpenGLRenderer::SceneData> OpenGLRenderer::s_SceneData = CreateScope<OpenGLRenderer::SceneData>();

	void OpenGLRenderer::Init()
	{
		RenderCommand::Init();
		Renderer2D::Init();
		Renderer3D::Init();

		init_immediate_renderer();
	}

	void OpenGLRenderer::Shutdown()
	{
		Renderer2D::Shutdown();
	}

	void OpenGLRenderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}

	void OpenGLRenderer::BeginScene(Camera::Camera& camera)
	{
		s_SceneData->ViewProjectMatrix = camera.GetViewProjectionMatrix();
	}

	void OpenGLRenderer::EndScene()
	{
	}

	void OpenGLRenderer::Submit(
		const Ref<Shader>& shader,
		const Ref<VertexArray>& vertexArray,
		const glm::mat4& transform
	)
	{
		shader->Bind();
		shader->SetMat4("u_ViewProjection", s_SceneData->ViewProjectMatrix);
		shader->SetMat4("u_Transform", transform);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
}