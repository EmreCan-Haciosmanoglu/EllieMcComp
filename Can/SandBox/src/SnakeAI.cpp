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


	int x = std::rand() % 11;
	int y = std::rand() % 11;
	while (x == 5 && y == 5)
	{
		x = std::rand() % 11;
		y = std::rand() % 11;
	}

	m_State[y][x] = 1;

	m_Snake.push_back({ 5, 5 });
	m_State[5][5] = -1;

	int nodes[4] = { 11 * 11 + 2 , 50 , 20, 4 };
	nn = new NeuralNetwork(nodes, 4, 0.05f);
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

