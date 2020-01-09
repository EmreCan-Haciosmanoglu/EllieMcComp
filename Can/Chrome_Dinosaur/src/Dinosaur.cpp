#include "canpch.h"
#include "GameLayer.h"
#include "Dinosaur.h"

Dinosaur::Dinosaur(float smallUp, float bigUp, float g, NeuralNetwork* brain, GameLayer* gameLayer)
	: m_SmallUp(smallUp)
	, m_BigUp(bigUp)
	, m_Y(0.0f)
	, m_G(g)
	, m_Brain(brain)
	, m_GameLayer(gameLayer)
{
}

Dinosaur::~Dinosaur()
{
}

void Dinosaur::Update(float ts, float* state, int stateSize)
{
	Move(ts, state, stateSize);
	if (Crushed(state, stateSize))
	{
		b_Dead = true;
		m_GameLayer->DinosaurIsDied();
	}
}

void Dinosaur::Draw(const glm::vec3& pos)
{
	
	Can::Renderer2D::DrawQuad({ pos.x , pos.y + m_Y + 0.5f, pos.z }, { 1.0f, 1.0f }, m_GameLayer->m_DinosaurImage);
}

void Dinosaur::Move(float ts, float* state, int stateSize)
{
	m_SpeedY += ts * m_G;
	m_Y += ts * m_SpeedY;

	if (m_Y <= 0.0f)
	{
		m_Y = 0.0f;
		m_SpeedY = 0.0f;
	}
	if (m_Y <= 0.0f)
	{
		int result = Think(state, stateSize);
		m_SpeedY += result == 0 ? 0.0f : result == 1 ? m_SmallUp : m_BigUp;
	}
}

int Dinosaur::Think(float* state, int size)
{
	Matrix* input = new Matrix(size, 1, state);
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

bool Dinosaur::Crushed(float* state, int stateSize)
{
	float x1 = state[0];
	float w1 = state[1];
	float h1 = state[2];
	float x2 = state[3];
	float w2 = state[4];
	float h2 = state[5];

	bool result1 = (x1 - w1 / 2 < 0.0f && 0.0f < x1 + w1 / 2) && m_Y < h1;
	bool result2 = (x2 - w2 / 2 < 0.0f && 0.0f < x2 + w2 / 2) && m_Y < h2;

	return result1 || result2;
}
