#pragma once
#include "Can.h"
#include "../temps/NeuralNetwork.h"

class GameLayer;

class Fish
{
public:
	Fish(float smallUp, float bigUp, float x, float y, float g, NeuralNetwork* brain, GameLayer* gameLayer);
	~Fish();

	void Update(float ts, float* state, int stateSize);
	void Draw(float height, int index);

	inline int GetPoints() { return m_Points; }
	inline NeuralNetwork* GetBrain() { return m_Brain; }
	inline bool IsDead() { return b_Dead; }
	inline void AddPoint() { m_Points++; }
private:
	void Move(float ts, float* state, int stateSize);
	int Think(float* state, int stateSize);
	bool Crushed(float* state, int stateSize);
private:
	int m_Points = 1;

	float m_SmallUp;
	float m_BigUp;
	float m_X;
	float m_Y;
	float m_SpeedY = 0.0f;
	float m_G;

	bool b_Dead = false;

	NeuralNetwork* m_Brain;
	GameLayer* m_GameLayer;
};