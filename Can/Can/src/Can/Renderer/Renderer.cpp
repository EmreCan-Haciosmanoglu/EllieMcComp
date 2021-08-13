#include "canpch.h"
#include "Renderer.h"
#include "Renderer2D.h"
#include "Renderer3D.h"

#include "Can/Immediate_Renderer/Immediate_Renderer.h"

#include "Platform/OpenGl/OpenGLShader.h"


namespace Can
{
	Scope<Renderer::SceneData> Renderer::s_SceneData = CreateScope<Renderer::SceneData>();

	void Renderer::Init()
	{
		CAN_PROFILE_FUNCTION();

		RenderCommand::Init();
		Renderer2D::Init();
		Renderer3D::Init();

		init_immediate_renderer();
	}

	void Renderer::Shutdown()
	{
		Renderer2D::Shutdown();
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
		shader->SetMat4("u_ViewProjection", s_SceneData->ViewProjectMatrix);
		shader->SetMat4("u_Transform", transform);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
}