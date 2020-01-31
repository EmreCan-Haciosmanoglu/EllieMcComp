#pragma once
#include "Can.h"
#include "imgui.h"

#include "../temps/NeuralNetwork.h"
#include "Player.h"

typedef std::function<bool(std::pair<std::array<float, STATE_SIZE>, int>, std::pair<std::array<float, STATE_SIZE>, int>)> Comparator;

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
	void AddData(std::array<float, STATE_SIZE> data);
private:
	void UpdateGame(float ts);
	void DrawGame();
	void DrawToLabel();
	void LabeTheData(std::array<float, 5> label);
	void NextGeneration();
	void Test();
	void Train();
	void NewBrain();

public:
	float m_LastPerc = 0.0f;
	int m_TestingCount = 100;
	int m_TrainingCount = 100;
	Comparator compFunctor =
		[](std::pair<std::array<float, STATE_SIZE>, int> elem1, std::pair<std::array<float, STATE_SIZE>, int> elem2)
	{
		return elem1.second > elem2.second;
	};

	int m_LabelIndex = 0;
	int m_GameWidth = 10;
	int m_GameHeight = 20;
	std::map<std::array<float, STATE_SIZE>,std::array<float,5>> m_LabeledData;
	std::vector<std::pair<std::array<float, STATE_SIZE>, int>> m_UnlabeledData;
	NeuralNetwork* m_Brain;

private:
	bool b_First = true;
	int m_CurrenPlayerIndex = 0;
	bool label = false;
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
		},{
		{ 1, 0 },
		{ 1, 1 },
		{ 1, 0 }
	} };

	int m_PlayerCount;
	int m_PlayerLeft;

	int m_MaxPoint = 0;
	int m_CurrentMaxPoint = 0;
	int m_Generation = 0;



	Player** m_Players;
};