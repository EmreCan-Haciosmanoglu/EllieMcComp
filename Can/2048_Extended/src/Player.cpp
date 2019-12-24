#include "canpch.h"
#include "Player.h"
#include "GameLayer.h"

Player::Player(NeuralNetwork* brain, Generation* parent, int dimensions)
	: m_Brain(brain)
	, m_Parent(parent)
	, m_Dimensions(dimensions)
	, m_InputSize(dimensions* dimensions)
{
	InitState();
}
Player::~Player()
{
	delete m_State;
}

void Player::Update(int step)
{
	m_Time++;
	glm::vec2 direction = Think(step);
	Play(direction);
	if (!SpaceExist())
	{
		b_IsDeath = true;
		m_Parent->PlayerDied();
		//m_Punishment = MoveExist();
		return;
	}
	AddCard();
}

int Player::GetPoints()
{
	int sum = 0;
	for (int i = 0; i < m_InputSize; i++)
	{
		sum += std::pow(2, m_State[i]);
	}
	int max = 0;
	for (int i = 0; i < m_InputSize; i++)
	{
		max = std::max(max, (int)std::pow(2, m_State[i]));
	}
	return sum + m_Time + max;
	//return m_Punishment ? (int)(std::max(0,sum-100)) : sum;
}

void Player::Draw(glm::vec2 offset)
{
	for (int x = 0; x < m_Dimensions; x++)
		for (int y = 0; y < m_Dimensions; y++)
			Can::Renderer2D::DrawQuad({ x, y }, { 0.9f, 0.9f }, m_Parent->GetCards()[m_State[x + y * m_Dimensions]]);
}

void Player::Play(glm::vec2 direction)
{
	if (direction.x < 0)
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
	else if (direction.x > 0)
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
	else if (direction.y < 0)
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
	else if (direction.y > 0)
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

int* Player::PlayFake(int* state, glm::vec2 direction)
{
	int size = m_Dimensions * m_Dimensions;
	int* Result = new int[size];
	for (int i = 0; i < size; i++)
		Result[i] = state[i];

	if (direction.x < 0)
	{
		for (int y = 0; y < m_Dimensions; y++)
		{
			for (int x = 1; x < m_Dimensions; x++)
			{
				if (Result[x + y * m_Dimensions] > 0)
				{
					while (x > 0 && Result[(x - 1) + y * m_Dimensions] == 0)
					{
						Result[(x - 1) + y * m_Dimensions] = Result[x + y * m_Dimensions];
						Result[x + y * m_Dimensions] = 0;
						x--;
					}
					if (x > 0 && Result[x + y * m_Dimensions] == Result[(x - 1) + y * m_Dimensions])
					{
						Result[(x - 1) + y * m_Dimensions] ++;
						Result[x + y * m_Dimensions] = 0;
					}
				}
			}
		}
	}
	else if (direction.x > 0)
	{
		for (int y = 0; y < m_Dimensions; y++)
		{
			for (int x = m_Dimensions - 2; x >= 0; x--)
			{
				if (Result[x + y * m_Dimensions] > 0)
				{
					while (x < m_Dimensions - 1 && Result[(x + 1) + y * m_Dimensions] == 0)
					{
						Result[(x + 1) + y * m_Dimensions] = Result[x + y * m_Dimensions];
						Result[x + y * m_Dimensions] = 0;
						x++;
					}
					if (x < m_Dimensions - 1 && Result[x + y * m_Dimensions] == Result[(x + 1) + y * m_Dimensions])
					{
						Result[(x + 1) + y * m_Dimensions] ++;
						Result[x + y * m_Dimensions] = 0;
					}
				}
			}
		}
	}
	else if (direction.y < 0)
	{
		for (int x = 0; x < m_Dimensions; x++)
		{
			for (int y = 1; y < m_Dimensions; y++)
			{
				if (Result[x + y * m_Dimensions] > 0)
				{
					while (y > 0 && Result[x + (y - 1) * m_Dimensions] == 0)
					{
						Result[x + (y - 1) * m_Dimensions] = Result[x + y * m_Dimensions];
						Result[x + y * m_Dimensions] = 0;
						y--;
					}
					if (y > 0 && Result[x + y * m_Dimensions] == Result[x + (y - 1) * m_Dimensions])
					{
						Result[x + (y - 1) * m_Dimensions] ++;
						Result[x + y * m_Dimensions] = 0;
					}
				}
			}
		}
	}
	else if (direction.y > 0)
	{
		for (int x = 0; x < m_Dimensions; x++)
		{
			for (int y = m_Dimensions - 2; y >= 0; y--)
			{
				if (Result[x + y * m_Dimensions] > 0)
				{
					while (y < m_Dimensions - 1 && Result[x + (y + 1) * m_Dimensions] == 0)
					{
						Result[x + (y + 1) * m_Dimensions] = Result[x + y * m_Dimensions];
						Result[x + y * m_Dimensions] = 0;
						y++;
					}
					if (y < m_Dimensions - 1 && Result[x + y * m_Dimensions] == Result[x + (y + 1) * m_Dimensions])
					{
						Result[x + (y + 1) * m_Dimensions] ++;
						Result[x + y * m_Dimensions] = 0;
					}
				}
			}
		}
	}

	if(SpaceExist(Result))
		AddFakeCard(Result);
	return Result;
}

void Player::InitState()
{
	m_State = new int[m_InputSize];
	for (int i = 0; i < m_InputSize; i++)
	{
		m_State[i] = 0;
	}
	AddCard();
	AddCard();
}

void Player::AddCard()
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

void Player::AddFakeCard(int* state)
{
	int x = std::rand() % m_Dimensions;
	int y = std::rand() % m_Dimensions;
	while (state[x + y * m_Dimensions] != 0)
	{
		x = std::rand() % m_Dimensions;
		y = std::rand() % m_Dimensions;
	}
	int i = std::rand() % 100;

	state[x + y * m_Dimensions] = i > 90 ? 2 : 1;
}

bool Player::MoveExist()
{
	for (int x = 0; x < m_Dimensions - 1; x++)
		for (int y = 0; y < m_Dimensions - 1; y++)
			if (m_State[x + y * m_Dimensions] == m_State[(x + 1) + y * m_Dimensions] || m_State[x + y * m_Dimensions] == m_State[x + (y + 1) * m_Dimensions])
				return true;
	return false;
}

bool Player::SpaceExist()
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

bool Player::SpaceExist(int* state)
{
	for (int i = 0; i < m_InputSize; i++)
	{
		if (state[i] == 0)
		{
			return true;
		}
	}
	return false;
}

int Player::Sum(int* state, int size)
{
	int sum = 0;
	for (int i = 0; i < size; i++)
		sum += state[i];
	return sum;
}

int* Player::Foresee(int* state, int dimension, int step)
{
	int size = std::pow(4, step-1);
	int resultSize = size * 4;
	int* result = new int[resultSize];

	int* state0 = PlayFake(state, m_Parent->GetDirection(0));
	int* state1 = PlayFake(state, m_Parent->GetDirection(1));
	int* state2 = PlayFake(state, m_Parent->GetDirection(2));
	int* state3 = PlayFake(state, m_Parent->GetDirection(3));

	if (step <= 1)
	{
		result[0] = Sum(state0, dimension * dimension);
		result[1] = Sum(state1, dimension * dimension);
		result[2] = Sum(state2, dimension * dimension);
		result[3] = Sum(state3, dimension * dimension);
	}
	else
	{
		int offset = 0;

		int* result0 = Foresee(state0, dimension, step - 1);
		for (int i = 0; i < size; i++)
		{
			result[offset] = result0[i];
			offset++;
		}
		delete[] result0;

		int* result1 = Foresee(state1, dimension, step - 1);
		for (int i = 0; i < size; i++)
		{
			result[offset] = result1[i];
			offset++;
		}
		delete[] result1;

		int* result2 = Foresee(state2, dimension, step - 1);
		for (int i = 0; i < size; i++)
		{
			result[offset] = result2[i];
			offset++;
		}
		delete[] result2;

		int* result3 = Foresee(state3, dimension, step - 1);
		for (int i = 0; i < size; i++)
		{
			result[offset] = result3[i];
			offset++;
		}
		delete[] result3;
	}
	delete[] state0;
	delete[] state1;
	delete[] state2;
	delete[] state3;

	return result;
}

glm::vec2 Player::Think(int step)
{
	int size = std::pow(4, step);
	//float* state = new float[m_InputSize + size];
	float* state = new float[m_InputSize];
	int index = 0;
	for (int i = 0; i < m_InputSize; i++)
	{
		state[index] = std::pow(2, m_State[i]);
		index++;
	}
	/*int* foreseeResults = Foresee(m_State, m_Dimensions, step);
	for (int i = 0; i < size; i++)
	{
		state[index] = foreseeResults[i];
		index++;
	}
	delete[] foreseeResults;*/
	Matrix* input = new Matrix(m_InputSize + size, 1, state);
	Matrix* result = m_Brain->FeedForward(input);

	float A[3] = {
		result->data[0],
		result->data[1],
		result->data[2]
	};
	float* maxElement = std::max_element(A, A + 3);
	_int64 dist = std::distance(A, maxElement);

	delete result;
	delete[] state;

	return m_Parent->GetDirection(dist);
}
