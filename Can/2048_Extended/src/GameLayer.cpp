#include "canpch.h"
#include "GameLayer.h"

GameLayer::GameLayer()
	:Layer("2048-Extended Game")
	, m_CameraController(1920.0f / 1080.0f)
	, m_Generation(new Generation(200,3))
	, m_Game(new SingleBrain(100,4))
{
}

void GameLayer::OnAttach() {}

void GameLayer::OnDetach() {}

void GameLayer::OnUpdate(Can::TimeStep ts)
{
	/*static float time = 0.0f;
	time += ts;
	if (time > 1.0f)
	{
		time -= 1.0f;
	}*/
	//m_Generation->Tick();
	m_Game->Tick();
	Can::RenderCommand::SetClearColor({ 0.9f, 0.9f, 0.9f, 1.0f });
	Can::RenderCommand::Clear();

	Can::Renderer2D::BeginScene(m_CameraController.GetCamera());

	//m_Generation->DrawBestPlayer();
	m_Game->DrawBestPuppet();
	Can::Renderer2D::EndScene();
}

void GameLayer::OnEvent(Can::Event::Event& event)
{
	m_CameraController.OnEvent(event);
}

void GameLayer::OnImGuiRender()
{
	ImGui::Begin("Current Data");
	ImGui::Text("%d Player Count", m_Game->GetPuppetCount());
	ImGui::Text("%d Player Left", m_Game->GetPuppetLeft());
	ImGui::Text("Max Score: %d", maxScore);
	ImGui::End();
}
