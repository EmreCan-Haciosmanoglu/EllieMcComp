#pragma once
#include "Can.h"
#include "imgui.h"

#include "../temps/NeuralNetwork.h"
#include "Player.h"

class GameLayer : public Can::Layer::Layer
{
public:
	GameLayer();
	virtual ~GameLayer() = default;

	virtual void OnAttach() override {}
	virtual void OnDetach() override {}

	virtual void OnUpdate(Can::TimeStep ts) override;
	virtual void OnEvent(Can::Event::Event& event) override;
	virtual void OnImGuiRender() override;

	void PlayerDied() { m_PlayerLeft--; }

	inline std::vector<std::vector<bool>> GetBlock(int index) { return m_Blocks[index]; }
private:
	void UpdateGame(float ts);
	void DrawGame();

private:
	int m_CurrenPlayerIndex = 0;
	bool m_IsChecked = true;
	Can::Camera::OrthographicCameraController m_CameraController;
	std::vector< std::vector<std::vector<bool>>> m_Blocks = { {
		{ 1 },
		{ 1 },
		{ 1 },
		{ 1 }
		},{
		{ 1 , 0 },
		{ 1 , 0 },
		{ 1 , 1 }
		},{
		{ 1 , 1 },
		{ 1 , 0 },
		{ 1 , 0 }
		},{
		{ 1 , 0 },
		{ 1 , 1 },
		{ 0 , 1 }
		},{
		{ 0 , 1 },
		{ 1 , 1 },
		{ 1 , 0 }
		},{
		{ 1, 1 },
		{ 1, 1 }
	} };

	int m_PlayerCount;
	int m_PlayerLeft;

	int m_MaxPoint = 0;
	int m_CurrentMaxPoint = 0;
	int m_Generation = 0;

	int m_GameWidth = 7;
	int m_GameHeight = 20;


	Player * *m_Players;
};