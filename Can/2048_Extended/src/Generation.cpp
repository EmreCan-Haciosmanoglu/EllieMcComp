#include"Generation.h"
#include <math.h> 
#include <map>

Generation::Generation(unsigned int playerCount, int step)
	: m_Dimension(4)
	, m_PlayerCount(playerCount)
	, m_PlayerLeft(playerCount)
	, m_Step(step)
	, m_Players(new Player * [m_PlayerCount])
{
	for (unsigned int i = 0; i < m_PlayerCount; i++)
		m_Players[i] = new Player(new NeuralNetwork(GetNodes(m_Step), 4, 0.05f),this,m_Dimension);

	m_Cards.push_back(Can::Texture2D::Create("assets/textures/_0.png"));
	m_Cards.push_back(Can::Texture2D::Create("assets/textures/_2.png"));
	m_Cards.push_back(Can::Texture2D::Create("assets/textures/_4.png"));
	m_Cards.push_back(Can::Texture2D::Create("assets/textures/_8.png"));
	m_Cards.push_back(Can::Texture2D::Create("assets/textures/_16.png"));
	m_Cards.push_back(Can::Texture2D::Create("assets/textures/_32.png"));
	m_Cards.push_back(Can::Texture2D::Create("assets/textures/_64.png"));
	m_Cards.push_back(Can::Texture2D::Create("assets/textures/_128.png"));
	m_Cards.push_back(Can::Texture2D::Create("assets/textures/_256.png"));
	m_Cards.push_back(Can::Texture2D::Create("assets/textures/_512.png"));
	m_Cards.push_back(Can::Texture2D::Create("assets/textures/_1024.png"));
	m_Cards.push_back(Can::Texture2D::Create("assets/textures/_2048.png"));
}

Generation::~Generation()
{
	m_Cards.clear();
	for (unsigned int i = 0; i < m_PlayerCount; i++)
		delete m_Players[i];

	delete[] m_Players;
}

void Generation::DrawBestPlayer()
{
	int* points = new int[m_PlayerCount];

	for (unsigned int i = 0; i < m_PlayerCount; i++)
		points[i] = m_Players[i]->GetPoints();

	int* maxElement = std::max_element(points, points + m_PlayerCount);
	_int64 index = std::distance(points, maxElement);

	m_Players[index]->Draw({ 0.0f, 0.0f});//?

	delete[] points;
}

void Generation::Tick()
{
	if (m_PlayerLeft <= 0)
	{
		NeuralNetwork** brains = new NeuralNetwork * [m_PlayerCount];
		std::map<int, NeuralNetwork*> b;
		int total = 0;
		for (unsigned int i = 0; i < m_PlayerCount; i++)
		{
			brains[i] = m_Players[i]->GetBrain();
			b[m_Players[i]->GetPoints()]= brains[i];
			total += m_Players[i]->GetPoints();
		}
		m_GenerationData = (--b.end())->first;
		int start = (int)((m_PlayerCount * 1.0f) / 100.0f);
		int middle = (int)((m_PlayerCount * 98.0f) / 100.0f);
		int end = start + middle;

		NeuralNetwork** newBrains = NeuralNetwork::Generate(b, total, m_PlayerCount, middle);
		int j = 0;
		for (int i = 0; i < start; i++)
		{
			delete brains[i];
			brains[i] = new NeuralNetwork(GetNodes(m_Step), 4, 0.05f);
		}
		for (int i = start; i < end; i++)
		{
			delete brains[i];
			brains[i] = newBrains[j]->Mutate(0.02f);
			j++;
		}

		m_PlayerLeft = m_PlayerCount;

		for (unsigned int i = 0; i < m_PlayerCount; i++)
			delete m_Players[i];


		for (unsigned int i = 0; i < m_PlayerCount; i++)
		{
			m_Players[i] = new Player(brains[i], this, m_Dimension);
			//m_Players[i] = new Player(new NeuralNetwork(GetNodes(), 5, 0.05f), this, m_Dimension);
		}
		m_GenerationIndex++;
	}
	else
	{
		for (unsigned int i = 0; i < m_PlayerCount; i++)
		{
			if (m_Players[i]->IsDeath())
				continue;
			m_Players[i]->Update(m_Step);
		}
	}
}