#include "canpch.h"
#include "Play.h"
#include "Game.h"

Play::Play(Game* game, glm::vec2 direction, int health)
	: m_CurrentDirection(direction)
	, m_Health(health)
	, m_Game(game)
{
	int nodes[4] = { 11 * 11 + 2 , 50 , 20, 4 };
	m_Brain = new NeuralNetwork(nodes, 4, 0.05f);
}

Play::~Play()
{
	delete m_Brain;
}

bool Play::Tick()
{
	return false;
}

bool Play::Update()
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

void Play::Draw(glm::vec2 offset)
{
	for (int y = -6; y < 5; y++)
	{
		for (int x = -5; x < 6; x++)
		{
			switch ((int)m_State[y + 6][x + 5])
			{
			case 1:
				Can::Renderer2D::DrawQuad({ x + offset.x, y + offset.y , 0.1f }, { 0.9f, 0.9f }, m_Game->m_RedColor);
				break;
			case -1:
				Can::Renderer2D::DrawQuad({ x + offset.x, y + offset.y , 0.1f }, { 0.9f, 0.9f }, m_Game->m_WhitishColor);
				break;
			default:
				break;
			}
		}
	}
}
