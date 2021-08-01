#include "canpch.h"
#include "Renderer3D.h"
#include "Can.h"

#include <glad/glad.h>


namespace Can
{
#define TERRAIN_SCALE_DOWN 10.0f
	std::vector<Object*> Renderer3D::s_Objects = {};

	void Renderer3D::Init()
	{
		CAN_PROFILE_FUNCTION();
	}
	void Renderer3D::Shutdown()
	{
		CAN_PROFILE_FUNCTION();
	}

	void Renderer3D::BeginScene(const Perspective_Camera& camera)
	{
		CAN_PROFILE_FUNCTION();

		for (Object* o : s_Objects)
		{
			if (!o->enabled)
				continue;
			Prefab* prefab = o->prefab;
			prefab->shader->Bind();
			prefab->shader->SetMat4("u_ViewProjection", camera.view_projection);
		}
	}
	void Renderer3D::EndScene()
	{
		CAN_PROFILE_FUNCTION();

	}

	void Renderer3D::AddObject(Object* obj)
	{
		CAN_PROFILE_FUNCTION();

		s_Objects.push_back(obj);
	}

	void Renderer3D::DeleteObject(Object* obj)
	{
		CAN_PROFILE_FUNCTION();

		auto e = std::find(s_Objects.begin(), s_Objects.end(), obj);

		if (e != s_Objects.end()) {
			s_Objects.erase(e);
		}
	}


	void Renderer3D::DrawObjects(const glm::mat4& lightspace, unsigned int shadowMapTexture, const Perspective_Camera& camera, const glm::vec3& lightPos)
	{
		CAN_PROFILE_FUNCTION();


		for (Object* obj : s_Objects)
		{
			if (!obj->enabled)
				continue;
			Prefab* prefab = obj->prefab;

			prefab->shader->Bind();
			glActiveTexture(GL_TEXTURE15);
			glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
			prefab->shader->SetMat4("u_LightSpace", lightspace);
			prefab->shader->SetMat4("u_Transform", obj->transform);
			prefab->shader->SetFloat4("u_TintColor", obj->tintColor);

			glm::vec3 rotatedLightRay = glm::rotate(lightPos, -(obj->rotation.y), glm::vec3{ 0.0f, 1.0f , 0.0f });
			prefab->shader->SetFloat3("u_LightPos", lightPos);
			prefab->shader->SetFloat3("u_ViewPos", camera.position);
			prefab->vertexArray->Bind();
			for (size_t i = 0; i < prefab->textureCount; i++)
				prefab->textures[i]->Bind((uint32_t)i);
			if (prefab->textures[0])
				for (size_t i = prefab->textureCount; i < MAX_TEXTURE_SLOTS; i++)
					prefab->textures[0]->Bind((uint32_t)i);

			RenderCommand::DrawIndexed(prefab->vertexArray);
		}
	}
}