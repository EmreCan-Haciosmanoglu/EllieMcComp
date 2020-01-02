#include "canpch.h"
#include "GameLayer.h"
#include "Fish.h"

Fish::Fish(float smallUp, float bigUp, float x, float y, float g, NeuralNetwork* brain, GameLayer* gameLayer)
	: m_SmallUp(smallUp)
	, m_BigUp(bigUp)
	, m_X(x)
	, m_Y(y)
	, m_G(g)
	, m_Brain(brain)
	, m_GameLayer(gameLayer)
{
}

Fish::~Fish()
{
}

void Fish::Update(float ts, float* state, int stateSize)
{
	Move(ts, state, stateSize);
	if (Crushed(state, stateSize))
	{
		b_Dead = true;
		m_GameLayer->FishIsDied();
	}
}

void Fish::Draw(float height, int index)
{
	Can::Renderer2D::DrawQuad({ m_X, m_Y - height / 2, 0.111f + index / 1000.f }, { 0.7f, 0.7f }, m_GameLayer->m_FishImage);
}

void Fish::Move(float ts, float* state, int stateSize)
{
	m_SpeedY += ts * m_G;
	if (m_SpeedY <= 0.0f)
	{
		int result = Think(state, stateSize);
		m_SpeedY += result == 0 ? 0.0f : result == 1 ? m_SmallUp : m_BigUp;
	}
	m_Y += ts * m_SpeedY;
}

int Fish::Think(float* state, int stateSize)
{
	int size = stateSize - 1 + 2;
	float* inputState = new float[size];
	for (int i = 0; i < stateSize - 1; i++)
	{
		inputState[i] = state[i];
	}
	inputState[stateSize - 1] = m_Y;
	inputState[stateSize - 0] = m_SpeedY;

	Matrix* input = new Matrix(size, 1, inputState);
	Matrix* result = m_Brain->FeedForward(input);

	float A[3] = {
		result->data[0],	// Don't Do anything
		result->data[1],	// Small Jump
		result->data[2]		// Big Jump
	};
	float* maxElement = std::max_element(A, A + 3);
	_int64 dist = std::distance(A, maxElement);

	delete result;

	return dist;
}

bool Fish::Crushed(float* state, int stateSize)
{
	bool result = false;

	result = result || ((state[0] - state[1] / 2 < m_X && state[0] + state[1] / 2 > m_X) && ((m_Y > state[4] - state[5] / 2) || (m_Y < state[2] + state[3] / 2)));
	result = result || m_Y > state[6] - 0.2f;
	result = result || m_Y < 0.2f;
	return result;
}
