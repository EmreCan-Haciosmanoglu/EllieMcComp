#include "canpch.h"
#include "Game.h"

Game::Game(unsigned int playCount)
	: m_PlayCount(playCount)
	, m_Plays(new Play*[m_PlayCount])
{
}

Game::~Game()
{
}

void Game::DrawBestPlay()
{
}

void Game::Tick()
{
	for (int i = 0; i < m_PlayCount; i++)
	{
		if (m_Plays[i]->IsDeath())
			continue;
	}

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
