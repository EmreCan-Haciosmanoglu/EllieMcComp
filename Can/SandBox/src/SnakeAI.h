#pragma once
#include "Can.h"

#include "imgui.h"

class SnakeAI : public Can::Layer::Layer
{
public:
	SnakeAI();
	virtual ~SnakeAI() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnUpdate(Can::TimeStep ts) override;
	virtual void OnEvent(Can::Event::Event& event) override;

private:
	bool UpdateGameState();
	void DrawGameState();

private:
	Can::Camera::OrthographicCameraController m_CameraController;

	glm::vec2 m_Offset = { -7.0f, +3.0f };

	glm::vec4 m_SquareColor = { 0.0f, 1.0f, 0.0f, 1.0f };
	glm::vec4 m_BlackColor = { 0.0f, 0.0f, 0.0f, 1.0f };
	glm::vec4 m_WhiteColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	glm::vec4 m_RedColor = { 1.0f, 0.0f, 0.0f, 1.0f };
	glm::vec4 m_WhitishColor = { 0.9f, 0.9f, 0.9f, 1.0f };

	int m_State[11][11] = {
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
	};

	std::vector<glm::vec2> m_Snake;
	glm::vec2 m_Directions[4] = { {0,1}, {1,0}, {0,-1}, {-1,0} };
	glm::vec2 m_CurrentDirection = m_Directions[0];
};