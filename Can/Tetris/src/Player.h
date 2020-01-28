#pragma once
#include "Can.h"
#include "../temps/NeuralNetwork.h"

class GameLayer;

class Player {
public:
	Player(int width, int height, NeuralNetwork* brain, GameLayer* parent);
	~Player();

	void Update(float ts);
	void Draw(const glm::vec2& offset);

	inline int GetPoint() { return m_Point; }
	inline bool IsDead() { return m_IsDead; }
	inline NeuralNetwork* GetBrain() { return m_Brain; }

	void MoveBlockDown();
	void MoveHorizontal(bool isLeft);
	void Rotate();
private:
	void NewBlock();
	bool MoveDown();
	bool WillCrush(const glm::vec2& direction);
	int Think();
	void IsGameOver();
	void BlockToState();
	void BreakFullRows();
private:
	bool m_IsDead = false;
	int m_Point = 1;
	size_t m_Width;
	size_t m_Height;

	int m_CurrentX;
	int m_CurrentY;

	int m_MaxCount = (m_Width - 1) + 2;
	int m_Counter = 0;

	#define BLOCK_QUEUE_SIZE  4

	int m_BlockIndex = 0;
	int m_BlockQueue[BLOCK_QUEUE_SIZE] = {-1, -1, -1, -1 };


	std::vector<std::vector<bool>> m_State;

	NeuralNetwork* m_Brain;
	GameLayer* m_Parent;

	std::vector<std::vector<bool>> m_CurrentBlock;
};