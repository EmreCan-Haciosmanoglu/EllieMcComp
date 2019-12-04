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

	std::srand(std::time(nullptr));

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

	int nodes[4] = { 11 * 11 + 2 , 50 , 10, 4 };
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

		int size = 11 * 11 + 2;

		float state[11 * 11 + 2];

		for (int i = 0; i < 11; i++)
		{
			for (int j = 0; j < 11; j++)
			{
				state[i * 11 + j] = (m_State[i][j]);
			}
		}
		state[11 * 11] = m_Snake.back().x;
		state[11 * 11 + 1] = m_Snake.back().y;

		Matrix* input = new Matrix(size, 1, state);
		Matrix* result = nn->FeedForward(input);
		result->Print();

		float A[4] = {
			result->data[0],
			result->data[1],
			result->data[2],
			result->data[3]
		};
		float* maxElement = std::max_element(A, A + 4);
		int dist = std::distance(A, maxElement);
		m_CurrentDirection = m_Directions[dist];
		delete result;

		stop = UpdateGameState();
	}

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
			switch ((int)m_State[y + 6][x + 5])
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
