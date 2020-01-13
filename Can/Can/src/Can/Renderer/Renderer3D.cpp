#include "canpch.h"
#include "Renderer3D.h"

#include "RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>


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
			o->S->Bind();
			o->S->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
		}
	}
	void Renderer3D::EndScene()
	{
	}

	void Renderer3D::AddObject(Object* obj)
	{
		s_Objects.push_back(obj);
	}

	void Renderer3D::DrawObjects()
	{
		for (Object* obj : s_Objects)
		{
			if (!obj->isEnabled)
				continue;
			if (obj->T)
				obj->T->Bind();
			obj->S->Bind();
			obj->S->SetMat4("u_Transform", obj->transform);

			obj->VA->Bind();
			RenderCommand::DrawIndexed(obj->VA);
		}
	}
}