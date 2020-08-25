#include "canpch.h"
#include "SandBox2DLayer.h"
#include <glm/gtc/type_ptr.hpp>

SandBox2DLayer::SandBox2DLayer()
	:Layer("2DLayer")
	, m_CameraController(1280.0f / 720.0f, true)
{
}

void SandBox2DLayer::OnAttach()
{
	m_Tex = Can::Texture2D::Create("assets/textures/Name.png");
}
void SandBox2DLayer::OnDetach()
{
}

void SandBox2DLayer::OnUpdate(Can::TimeStep ts)
{
	CAN_PROFILE_FUNCTION();

	m_CameraController.OnUpdate(ts);

	Can::RenderCommand::SetClearColor({ 0.15f, 0.15f, 0.15f, 1.0f });
	Can::RenderCommand::Clear();

	Can::Renderer2D::BeginScene(m_CameraController.GetCamera());

	Can::Renderer2D::DrawQuad(Can::DrawQuadParameters{ { 0.0f, -0.2f, 0.0f }, { 1.0f, 1.0f }, 0.0f, { 0.8f, 0.2f, 0.3f, 1.0f } });
	Can::Renderer2D::DrawQuad(Can::DrawQuadParameters{ { -0.5f, 1.5f, 0.0f }, { 1.5f, 0.5f }, 0.0f, { 0.2f, 0.8f, 0.3f, 1.0f } });
	Can::Renderer2D::DrawQuad(Can::DrawQuadParameters{ { 2.3f, -1.0f, 0.0f }, { 0.5f, 5.0f }, 0.0f, { 0.8f, 0.3f, 0.8f, 1.0f } });

	//Can::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 5.0f, 5.0f }, m_Tex);

	Can::Renderer2D::EndScene();
}

void SandBox2DLayer::OnEvent(Can::Event::Event& event)
{
	m_CameraController.OnEvent(event);
}

void SandBox2DLayer::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));

	for (auto& result : m_ProfileResults)
	{
		char label[50];
		strcpy(label, result.Name);
		strcat(label, "  %.6fns");
		ImGui::Text("", result.Time);
	}

	ImGui::End();
}
