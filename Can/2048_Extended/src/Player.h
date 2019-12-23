#pragma once
#include "Can.h"
#include "../temps/NeuralNetwork.h"
#include "imgui.h"

class Generation;

class Player {
public:
	Player(NeuralNetwork* brain, Generation* parent, int dimensions);
	~Player();

	void Play(glm::vec2 direction);
	void Draw(glm::vec2 offset);
	void Update(int step);

	int GetPoints();
	inline int GetDimensions() const { return m_Dimensions; }
	inline NeuralNetwork* GetBrain() { return m_Brain; }
	inline bool IsDeath() const { return b_IsDeath; }

private:
	int* PlayFake(int* state, glm::vec2 direction);
	void InitState();
	void AddCard();
	void AddFakeCard(int* state);
	bool MoveExist();
	bool SpaceExist();
	bool SpaceExist(int* state);
	int Sum(int* state, int size);
	int* Foresee(int* state, int dimension, int step);
	glm::vec2 Think(int step);
private:

	int* m_State;
	int m_Dimensions;
	int m_InputSize;
	int m_Time = 0;

	bool m_Punishment = false;
	bool b_IsDeath = false;

	NeuralNetwork* m_Brain;
	Generation* m_Parent;
};