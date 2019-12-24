#include "canpch.h"
#include "Puppet.h"
#include "GameLayer.h"

Puppet::Puppet(SingleBrain* parent, int dimensions)
	: m_Parent(parent)
	, m_Dimensions(dimensions)
	, m_InputSize(dimensions* dimensions)
{
	InitState();
}

Puppet::~Puppet()
{
	delete m_State;
}

void Puppet::Update()
{
	int direction = Think();
	float* copyState = GetCopyState();
	Play(direction);

	if (!SpaceExist())
	{
		b_IsDeath = true;
		m_Parent->PuppetDied();
		bool doesExist = MoveExist();
		if (doesExist)
		{
			Matrix* input = new Matrix(m_InputSize, 1, copyState);
			float t[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

			t[direction] = 0.0f;

			Matrix* target = new Matrix(4, 1, t);
			m_Parent->GetTheBrain()->Train(input, target);
		}
		return;
	}
	else
	{
		if (Sum(m_State, m_InputSize) > Sum(copyState, m_InputSize))
		{
			Matrix* input = new Matrix(m_InputSize, 1, copyState);
			float t[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

			t[direction] = 1.0f;

			Matrix* target = new Matrix(4, 1, t);
			m_Parent->GetTheBrain()->Train(input, target);
		}
	}
	AddCard();
	delete[] copyState;
}

int Puppet::Sum(int* state, int size)
{
	int sum = 0;
	for (int i = 0; i < size; i++)
		sum += state[i];
	return sum;
}
int Puppet::Sum(float* state, int size)
{
	float sum = 0;
	for (int i = 0; i < size; i++)
		sum += state[i];
	return sum;
}

int Puppet::GetPoints()
{
	int max = 0;
	for (int i = 0; i < m_InputSize; i++)
	{
		max = std::max(max, m_State[i]);
	}
	int sum = 0;
	for (int i = 0; i < m_InputSize; i++)
	{
		sum += std::pow(2, m_State[i]);
	}
	return sum;
	//return max;
	//return std::pow(2,max);
}

void Puppet::Draw()
{
	for (int x = 0; x < m_Dimensions; x++)
		for (int y = 0; y < m_Dimensions; y++)
			Can::Renderer2D::DrawQuad({ x, y }, { 0.9f, 0.9f }, m_Parent->GetCards()[m_State[x + y * m_Dimensions]]);
}

void Puppet::Play(int direction)
{
	if (direction == 1)
	{
		for (int y = 0; y < m_Dimensions; y++)
		{
			for (int x = 1; x < m_Dimensions; x++)
			{
				if (m_State[x + y * m_Dimensions] > 0)
				{
					while (x > 0 && m_State[(x - 1) + y * m_Dimensions] == 0)
					{
						m_State[(x - 1) + y * m_Dimensions] = m_State[x + y * m_Dimensions];
						m_State[x + y * m_Dimensions] = 0;
						x--;
					}
					if (x > 0 && m_State[x + y * m_Dimensions] == m_State[(x - 1) + y * m_Dimensions])
					{
						m_State[(x - 1) + y * m_Dimensions] ++;
						m_State[x + y * m_Dimensions] = 0;
					}
				}
			}
		}
	}
	else if (direction == 3)
	{
		for (int y = 0; y < m_Dimensions; y++)
		{
			for (int x = m_Dimensions - 2; x >= 0; x--)
			{
				if (m_State[x + y * m_Dimensions] > 0)
				{
					while (x < m_Dimensions - 1 && m_State[(x + 1) + y * m_Dimensions] == 0)
					{
						m_State[(x + 1) + y * m_Dimensions] = m_State[x + y * m_Dimensions];
						m_State[x + y * m_Dimensions] = 0;
						x++;
					}
					if (x < m_Dimensions - 1 && m_State[x + y * m_Dimensions] == m_State[(x + 1) + y * m_Dimensions])
					{
						m_State[(x + 1) + y * m_Dimensions] ++;
						m_State[x + y * m_Dimensions] = 0;
					}
				}
			}
		}
	}
	else if (direction == 0)
	{
		for (int x = 0; x < m_Dimensions; x++)
		{
			for (int y = 1; y < m_Dimensions; y++)
			{
				if (m_State[x + y * m_Dimensions] > 0)
				{
					while (y > 0 && m_State[x + (y - 1) * m_Dimensions] == 0)
					{
						m_State[x + (y - 1) * m_Dimensions] = m_State[x + y * m_Dimensions];
						m_State[x + y * m_Dimensions] = 0;
						y--;
					}
					if (y > 0 && m_State[x + y * m_Dimensions] == m_State[x + (y - 1) * m_Dimensions])
					{
						m_State[x + (y - 1) * m_Dimensions] ++;
						m_State[x + y * m_Dimensions] = 0;
					}
				}
			}
		}
	}
	else if (direction == 2)
	{
		for (int x = 0; x < m_Dimensions; x++)
		{
			for (int y = m_Dimensions - 2; y >= 0; y--)
			{
				if (m_State[x + y * m_Dimensions] > 0)
				{
					while (y < m_Dimensions - 1 && m_State[x + (y + 1) * m_Dimensions] == 0)
					{
						m_State[x + (y + 1) * m_Dimensions] = m_State[x + y * m_Dimensions];
						m_State[x + y * m_Dimensions] = 0;
						y++;
					}
					if (y < m_Dimensions - 1 && m_State[x + y * m_Dimensions] == m_State[x + (y + 1) * m_Dimensions])
					{
						m_State[x + (y + 1) * m_Dimensions] ++;
						m_State[x + y * m_Dimensions] = 0;
					}
				}
			}
		}
	}
}

void Puppet::InitState()
{
	m_State = new int[m_InputSize];
	for (int i = 0; i < m_InputSize; i++)
	{
		m_State[i] = 0;
	}
	AddCard();
	AddCard();
}

void Puppet::AddCard()
{
	int x = std::rand() % m_Dimensions;
	int y = std::rand() % m_Dimensions;
	while (m_State[x + y * m_Dimensions] != 0.0f)
	{
		x = std::rand() % m_Dimensions;
		y = std::rand() % m_Dimensions;
	}
	int i = std::rand() % 100;

	m_State[x + y * m_Dimensions] = i > 90 ? 2 : 1;
}

bool Puppet::MoveExist()
{
	for (int x = 0; x < m_Dimensions - 1; x++)
		for (int y = 0; y < m_Dimensions - 1; y++)
			if (m_State[x + y * m_Dimensions] == m_State[(x + 1) + y * m_Dimensions] || m_State[x + y * m_Dimensions] == m_State[x + (y + 1) * m_Dimensions])
				return true;
	return false;
}

bool Puppet::SpaceExist()
{
	for (int i = 0; i < m_InputSize; i++)
	{
		if (m_State[i] == 0)
		{
			return true;
		}
	}
	return false;
}

float* Puppet::GetCopyState()
{
	float* result = new float[m_InputSize];
	for (int i = 0; i < m_InputSize; i++)
		result[i] = m_State[i];
	return result;
}

int Puppet::Think()
{
	float* state = new float[m_InputSize];
	for (int i = 0; i < m_InputSize; i++)
	{
		state[i] = std::pow(2, m_State[i]);
	}
	Matrix* input = new Matrix(m_InputSize, 1, state);
	Matrix* result = m_Parent->GetTheBrain()->FeedForward(input);

	float A[4] = {
		result->data[0],
		result->data[1],
		result->data[2],
		result->data[3]
	};

	float* maxElement = std::max_element(A, A + 4);
	_int64 dist = std::distance(A, maxElement);

	delete result;
	delete[] state;

	return dist;
}