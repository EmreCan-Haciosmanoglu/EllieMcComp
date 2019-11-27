#include "canpch.h"
#include "SandBox3D.h"


SandBox3D::SandBox3D()
	: Layer("3D Layer")
	, m_CameraController(45.0f, 1280.0f / 720.0f, 0.1f, 30.0f)
{
}

void SandBox3D::OnAttach()
{
}
void SandBox3D::OnDetach()
{
}

void SandBox3D::OnUpdate(Can::TimeStep ts)
{
	m_CameraController.OnUpdate(ts);

	Can::RenderCommand::SetClearColor({ 0.15f, 0.15f, 0.15f, 1.0f });
	Can::RenderCommand::Clear();

	Can::Renderer3D::BeginScene(m_CameraController.GetCamera());

	Can::Renderer3D::DrawCube({ 0.0f, 0.0f, -4.0f }, { 0.5f, 0.5f, 0.5f });
	Can::Renderer3D::DrawCube({ 2.0f, 0.0f, -4.0f }, { 0.5f, 0.5f, 0.5f });
	Can::Renderer3D::DrawCube({ 0.0f, 2.0f, -4.0f }, { 0.5f, 0.5f, 0.5f });

	Can::Renderer2D::EndScene();
}

void SandBox3D::OnEvent(Can::Event::Event & event)
{
	m_CameraController.OnEvent(event);
}
