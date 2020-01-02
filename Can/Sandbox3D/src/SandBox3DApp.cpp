#include "canpch.h"
#include "SandBox3DApp.h"
#include "Can/EntryPoint.h"

Can::Application* Can::CreateApplication()
{
	return new Sandbox3D();
}

Sandbox3D::Sandbox3D()
	: m_Scene(new Scene(this))
	, m_Debug(new Debug(this))
{
	PushLayer(m_Scene);
	PushOverlay(m_Debug);
}

Sandbox3D::~Sandbox3D()
{
}
