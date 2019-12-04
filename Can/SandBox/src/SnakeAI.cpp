#include "canpch.h"
#include "SnakeAI.h"

#include <glm/gtc/type_ptr.hpp>
#include <cstdlib>
#include <ctime>

SnakeAI::SnakeAI()
	:Layer("Snake Game")
	, m_CameraController(1920.0f / 1080.0f)
{
	float m_AspectRatio = 1920.0f / 1080.0f;
	float m_ZoomLevel = 10.0f;
	m_CameraController.GetCamera().SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);

	std::srand(std::time(nullptr));
	int x = std::rand() % 11;
	int y = std::rand() % 11;
	while (x == 5 && y == 5)
	{
		x = std::rand() % 11;
		y = std::rand() % 11;
	}

	m_State[y][x] = 1;

	m_Snake.push_back({ 5,5 });
	m_State[5][5] = -1;
}

void SnakeAI::OnAttach() {}
void SnakeAI::OnDetach() {}

void SnakeAI::OnUpdate(Can::TimeStep ts)
{
	//m_CameraController.OnUpdate(ts);
	static float time = 0.0f;
	static bool stop = false;

	

	time += ts;
	if (time >= 0.5f)
	{
		time -= 0.5f;
		stop = UpdateGameState();
	}
	if (stop)
		return;

	Can::RenderCommand::SetClearColor({ 0.0f, 0.0f, 0.0f, 1.0f });
	Can::RenderCommand::Clear();


	Can::Renderer2D::BeginScene(m_CameraController.GetCamera());

	DrawGameState();

	Can::Renderer2D::EndScene();
}

void SnakeAI::OnEvent(Can::Event::Event& event)
{
	m_CameraController.OnEvent(event);
}

bool SnakeAI::UpdateGameState()
{
	glm::vec2 tailPos = m_Snake.front();
	m_State[(int)tailPos.y][(int)tailPos.x] = 0;

	glm::vec2 position = m_Snake.back();
	glm::vec2 nextPos = position + m_CurrentDirection;
	if (nextPos.x < 0 || nextPos.y < 0 || nextPos.x > 10 || nextPos.y > 10)
		return true;
	m_Snake.push_back(nextPos);
	m_State[(int)nextPos.y][(int)nextPos.x] = -1;
	m_Snake.erase(m_Snake.begin());
	return false;
}

void SnakeAI::DrawGameState()
{
	for (int y = -6; y < 5; y++)
	{
		for (int x = -5; x < 6; x++)
		{
			switch (m_State[y + 6][x + 5])
			{
			case 1:
				Can::Renderer2D::DrawQuad({ x + m_Offset.x, y + m_Offset.y , 0.1f }, { 0.9f, 0.9f }, m_RedColor);
				break;
			case -1:
				Can::Renderer2D::DrawQuad({ x + m_Offset.x, y + m_Offset.y , 0.1f }, { 0.9f, 0.9f }, m_WhitishColor);
				break;
			default:
				break;
			}
		}
	}
}
