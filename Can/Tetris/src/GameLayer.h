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

	inline bool** GetBlock(int index) { return (m_Blocks[index]); }
private:
	void UpdateGame(float ts);
	void DrawGame();

private:
	Can::Camera::OrthographicCameraController m_CameraController;

	int m_PlayerCount;
	int m_PlayerLeft;

	int m_MaxPoint = 0;
	int m_CurrentMaxPoint = 0;
	int m_Generation = 0;

	int m_GameWidth = 7;
	int m_GameHeight = 20;

	bool*** m_Blocks = new bool**[6]{ new bool*[4]{
		new bool[4] {true,false,false,false},
		new bool[4] {true,false,false,false},
		new bool[4] {true,false,false,false},
		new bool[4] {true,false,false,false}
		},new bool* [4]{
		new bool[4] {true ,false,false,false},
		new bool[4] {true ,false,false,false},
		new bool[4] {true ,true ,false,false},
		new bool[4] {false,false,false,false}
		},new bool* [4]{
		new bool[4] {true ,true ,false,false},
		new bool[4] {true ,false,false,false},
		new bool[4] {true ,false,false,false},
		new bool[4] {false,false,false,false}
		},new bool* [4]{
		new bool[4] {true ,false,false,false},
		new bool[4] {true ,true ,false,false},
		new bool[4] {false,true ,false,false},
		new bool[4] {false,false,false,false}
		},new bool* [4]{
		new bool[4] {false,true ,false,false},
		new bool[4] {true ,true ,false,false},
		new bool[4] {true ,false,false,false},
		new bool[4] {false,false,false,false}
		},new bool* [4]{
		new bool[4] {true ,true ,false,false},
		new bool[4] {true ,true ,false,false},
		new bool[4] {false,false,false,false},
		new bool[4] {false,false,false,false}
	} };

	Player * *m_Players;
};