#include "canpch.h"
#include "SnakeAI.h"

#include <glm/gtc/type_ptr.hpp>
#include <cstdlib>
#include <ctime>
#include <algorithm>  


SnakeAI::SnakeAI()
	:Layer("Snake Game")
	, m_CameraController(1920.0f / 1080.0f)
{
	float m_AspectRatio = 1920.0f / 1080.0f;
	float m_ZoomLevel = 10.0f;
	m_CameraController.GetCamera().SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);

}

void SnakeAI::OnAttach() {}
void SnakeAI::OnDetach() {}

void SnakeAI::OnUpdate(Can::TimeStep ts)
{
	//m_CameraController.OnUpdate(ts);
	static float time = 0.0f;
	static bool stop = false;

	if (stop)
		return;
	std::random_shuffle(indices.begin(), indices.end());


	time += ts;
	if (time >= 0.5f)
	{
		time -= 0.5f;
		m_Game->Tick();
	}

	Can::RenderCommand::SetClearColor({ 0.0f, 0.0f, 0.0f, 1.0f });
	Can::RenderCommand::Clear();


	Can::Renderer2D::BeginScene(m_CameraController.GetCamera());

	m_Game->DrawBestPlay();

	Can::Renderer2D::EndScene();
}

void SnakeAI::OnEvent(Can::Event::Event& event)
{
	m_CameraController.OnEvent(event);
}

