#pragma once
#include "Can.h"
#include "../temps/NeuralNetwork.h"

class GameLayer;

class Dinosaur
{
public:
	Dinosaur(float smallUp, float bigUp, float g, NeuralNetwork* brain, GameLayer* gameLayer);
	~Dinosaur();

	void Update(float ts, float* state, int stateSize);
	void Draw(const glm::vec3& pos);

	inline int GetPoints() { return m_Points; }
	inline NeuralNetwork* GetBrain() { return m_Brain; }
	inline bool IsDead() { return b_Dead; }
	inline void AddPoint() { m_Points++; }
private:
	void Move(float ts, float* state, int size);
	int Think(float* state, int stateSize);
	bool Crushed(float* state, int stateSize);
private:
	int m_Points = 1;

	float m_SmallUp;
	float m_BigUp;
	float m_Y;
	float m_SpeedY = 0.0f;
	float m_G;

	bool b_Dead = false;

	NeuralNetwork* m_Brain;
	GameLayer* m_GameLayer;
};