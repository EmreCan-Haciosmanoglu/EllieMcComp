#include "canpch.h"
#include "SingleBrain.h"

SingleBrain::SingleBrain(int puppetCount, int dimension)
	: m_Dimension(dimension)
	, m_PuppetCount(puppetCount)
	, m_PuppetLeft(puppetCount)
	, m_Puppets(new Puppet* [puppetCount])
	, m_TheBrain(new NeuralNetwork(GetNodes(), 4, 0.05f))
{
	for (unsigned int i = 0; i < m_PuppetCount; i++)
		m_Puppets[i] = new Puppet(this, m_Dimension);

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

SingleBrain::~SingleBrain()
{
	m_Cards.clear();
	for (unsigned int i = 0; i < m_PuppetCount; i++)
		delete m_Puppets[i];

	delete[] m_Puppets;
}

void SingleBrain::DrawBestPuppet()
{
	int* points = new int[m_PuppetCount];

	for (unsigned int i = 0; i < m_PuppetCount; i++)
		points[i] = m_Puppets[i]->GetPoints();

	int* maxElement = std::max_element(points, points + m_PuppetCount);
	_int64 index = std::distance(points, maxElement);
	maxScore = std::max(maxScore, *maxElement);
	currentScore = std::max(currentScore, *maxElement);
	m_Puppets[index]->Draw();//?

	delete[] points;
}

void SingleBrain::Tick()
{
	if (m_PuppetLeft <= 0)
	{
		m_PuppetLeft = m_PuppetCount;

		for (unsigned int i = 0; i < m_PuppetCount; i++)
			delete m_Puppets[i];

		for (unsigned int i = 0; i < m_PuppetCount; i++)
		{
			m_Puppets[i] = new Puppet(this, m_Dimension);
		}
		Generation++;
		currentScore = 0;
	}
	else
	{
		for (int i = 0; i < m_PuppetCount; i++)
		{
			if (m_Puppets[i]->IsDeath())
				continue;
			m_Puppets[i]->Update();
		}
	}
}
