#include "canpch.h"
#include "Renderer.h"

#include "Platform/OpenGl/OpenGLShader.h"

namespace Can
{
	Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;
	void Renderer::BeginScene(OrthographicCamera& camera)
	{
		m_SceneData->ViewProjectMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(
		const std::shared_ptr<Shader>& shader,
		const std::shared_ptr<VertexArray>& vertexArray,
		const glm::mat4& transform
	)
	{
		shader->Bind();
		std::shared_ptr<OpenGLShader> openglshader = std::dynamic_pointer_cast<OpenGLShader>(shader);
		if (openglshader)
		{
			openglshader->UploadUniformMat4("u_ViewProjection", m_SceneData->ViewProjectMatrix);
			openglshader->UploadUniformMat4("u_Transform", transform);
		}

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
}