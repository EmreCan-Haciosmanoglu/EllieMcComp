#include "canpch.h"
#include "ShadowMapEntityRenderer.h"
#include "Can/Renderer/Renderer3D.h"
#include "Can/Renderer/RenderCommand.h"
namespace Can
{
	ShadowMapEntityRenderer::ShadowMapEntityRenderer(const Ref<Shader>& shader, const glm::mat4& ls)
		: shader(shader)
		, ligthSpaceMatrix(ls)
	{
	}

	void ShadowMapEntityRenderer::Render()
	{
		for (Object* obj : Renderer3D::s_Objects)
		{
			if (!obj->enabled)
				continue;

			shader->SetMat4("model", obj->transform);
			obj->prefab->vertexArray->Bind();
			RenderCommand::DrawIndexed(obj->prefab->vertexArray);
		}
	}
}