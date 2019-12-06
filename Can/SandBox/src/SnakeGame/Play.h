#pragma once
#include "Can.h"
#include "../../temps/NeuralNetwork.h"
class Game;

class Play {
public:
	Play(Game* game, glm::vec2 direction, int health);
	~Play();

	void Tick();
	bool Update();
	void Draw(glm::vec2 offset);

	inline int GetPoints() const { return m_Points; }
	inline NeuralNetwork* GetBrain() { return m_Brain; }
	inline bool IsDeath() const { return m_Health <= 0 || b_IsDeath; }

private:

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
	int m_Points = 0;
	int m_Health;

	bool b_IsDeath = false;

	std::vector<glm::vec2> m_Snake;
	
	glm::vec2 m_CurrentDirection;

	Game* m_Game;
	NeuralNetwork* m_Brain;
};