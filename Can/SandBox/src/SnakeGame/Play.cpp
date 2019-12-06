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

void Play::Tick()
{

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

	int size = 11 * 11 + 2;
	Matrix* input = new Matrix(size, 1, state);
	Matrix* result = m_Brain->FeedForward(input);
	result->Print();

	float A[4] = {
		result->data[0],
		result->data[1],
		result->data[2],
		result->data[3]
	};
	float* maxElement = std::max_element(A, A + 4);
	int dist = std::distance(A, maxElement);
	m_CurrentDirection = m_Game->m_Directions[dist];
	delete result;

	b_IsDeath = Update();
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
