#pragma once
#include "Can.h"
#include "../temps/NeuralNetwork.h"

class GameLayer;

class Player {
public:
	Player(int width, int height, NeuralNetwork* brain, GameLayer* parent);
	~Player();

	void Update(float ts);
	void Draw();

	inline int GetPoint() { return m_Point; }
	inline bool IsDead() { return m_IsDead; }
	inline NeuralNetwork* GetBrain() { return m_Brain; }
private:
	bool m_IsDead = false;
	int m_Point = 1;
	int m_Width;
	int m_Heigth;
	int m_MaxCount = (m_Width - 1) + 2;
	int m_Counter = 0;

	std::vector<std::vector<bool>> m_State;

	NeuralNetwork* m_Brain;
	GameLayer* m_Parent;
};