#include "canpch.h"
#include "Renderer3D.h"
#include "Can.h"


namespace Can
{
	static std::vector<Object*> s_Objects;

	void Renderer3D::Init()
	{
		CAN_PROFILE_FUNCTION();

	}
	void Renderer3D::Shutdown()
	{
		CAN_PROFILE_FUNCTION();

	}

	void Renderer3D::BeginScene(const Camera::PerspectiveCamera& camera)
	{
		CAN_PROFILE_FUNCTION();

		for (Object* o : s_Objects)
		{
			if (!o->enabled)
				continue;
			o->prefab->shader->Bind();
			o->prefab->shader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
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

	void Renderer3D::DrawObjects()
	{
		CAN_PROFILE_FUNCTION();

		for (Object* obj : s_Objects)
		{
			if (!obj->enabled)
				continue;
			if (obj->prefab->texture)
				obj->prefab->texture->Bind();
			obj->prefab->shader->Bind();
			obj->prefab->shader->SetMat4("u_Transform", obj->transform);

			obj->prefab->vertexArray->Bind();
			RenderCommand::DrawIndexed(obj->prefab->vertexArray);
		}
	}
}