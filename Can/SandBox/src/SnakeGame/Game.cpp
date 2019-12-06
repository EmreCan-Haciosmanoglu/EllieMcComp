#include "canpch.h"
#include "Game.h"

Game::Game(unsigned int playCount)
	: m_PlayCount(playCount)
	, m_PlayLeft(m_PlayCount)
	, m_Plays(new Play*[m_PlayCount])
{
	for (int i = 0; i < m_PlayCount; i++)
	{
		m_Plays[i] = new Play(this, m_Directions[0], 20);
	}
}

Game::~Game()
{
	for (int i = 0; i < m_PlayCount; i++)
		delete m_Plays[i];

	delete[] m_Plays;
}

void Game::DrawBestPlay()
{
	int* points = new int[m_PlayCount];

	for (int i = 0; i < m_PlayCount; i++)
		points[i] = m_Plays[i]->GetPoints();

	int* maxElement = std::max_element(points, points + m_PlayCount);
	int index = std::distance(points, maxElement);

	m_Plays[index]->Draw(m_Offset);

	delete[] points;
	delete maxElement;
}

void Game::Tick()
{
	if (m_PlayLeft <= 0)
	{
		NeuralNetwork** brains = new NeuralNetwork * [m_PlayCount];
		float* points = new float[m_PlayCount];
		float total = 0;
		for (int i = 0; i < m_PlayCount; i++)
		{
			brains[i] = m_Plays[i]->GetBrain();
			points[i] = m_Plays[i]->GetPoints();
			total += points[i];
		}
		for (int i = 0; i < m_PlayCount; i++)
		{
			points[i] /= total;
			points[i] *= 100.0f;
		}

		brains = NeuralNetwork::Generate(brains, points, m_PlayCount, m_PlayCount);
		brains = NeuralNetwork::Mutate(brains, 0.01f);
		m_PlayLeft = m_PlayCount;

		delete[] points;
		return;
	}
	for (int i = 0; i < m_PlayCount; i++)
	{
		if (m_Plays[i]->IsDeath())
			continue;
		m_Plays[i]->Tick();
	}
}
