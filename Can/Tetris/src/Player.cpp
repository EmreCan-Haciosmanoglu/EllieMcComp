#include "canpch.h"
#include "Player.h"
#include "GameLayer.h"

Player::Player(int width, int height, NeuralNetwork* brain, GameLayer* parent)
	: m_Width(width)
	, m_Heigth(height)
	, m_Brain(brain)
	, m_Parent(parent)
{
	for (int i = 0; i < m_Heigth; i++)
	{
		m_State.push_back({});

		for (int j = 0; j < m_Width; j++)
			m_State[i].push_back(false);

	}
}

void Player::Update(float ts)
{
	m_Counter++;
	if (m_Counter == m_MaxCount)
	{
		m_Counter = 0;

	}
}

void Player::Draw()
{
	for (int i = 0; i < m_Heigth; i++)
		for (int j = 0; j < m_Width; j++)
			if (m_State[i][j])
				Can::Renderer2D::DrawQuad({ 0.0f + j, 0.0f + i }, { 0.9f, 0.9f }, { 0.3f, 0.8f, 0.2f, 1.0f });
			else
				Can::Renderer2D::DrawQuad({ 0.0f + j, 0.0f + i }, { 0.9f, 0.9f }, { 0.9f, 0.9f, 0.9f, 1.0f });

}