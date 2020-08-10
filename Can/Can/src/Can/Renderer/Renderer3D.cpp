#include "canpch.h"
#include "Renderer3D.h"
#include "Can.h"


namespace Can
{
	static std::vector<Object*> s_Objects;

	void Renderer3D::Init()
	{
	}
	void Renderer3D::Shutdown()
	{
	}

	void Renderer3D::BeginScene(const Camera::PerspectiveCamera& camera)
	{
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
	}

	void Renderer3D::AddObject(Object* obj)
	{
		s_Objects.push_back(obj);
	}

	void Renderer3D::DeleteObject(Object* obj)
	{
		auto e = std::find(s_Objects.begin(), s_Objects.end(), obj);

		if (e != s_Objects.end()) {
			s_Objects.erase(e);
		}
	}

	void Renderer3D::DrawObjects()
	{
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