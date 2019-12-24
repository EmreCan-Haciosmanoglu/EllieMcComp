#pragma once
#include "Can.h"
#include "Player.h"

class Generation {
public:
	Generation(unsigned int playerCount, int step);
	~Generation();

	void DrawBestPlayer();
	void Tick();

	inline void PlayerDied() { m_PlayerLeft--; }

	inline std::vector<Can::Ref<Can::Texture2D>> GetCards() { return m_Cards; }

	inline int GetGenerationIndex() const { return m_GenerationIndex; }
	inline int GetGenerationData() const { return m_GenerationData; }
	inline int GetPlayerCount() const { return m_PlayerCount; }
	inline int GetPlayerLeft() const { return m_PlayerLeft; }
	inline int* GetNodes(int step) { int size = m_Dimension * m_Dimension; return new int[4]{ size, 20 , 10, 3 }; }
	//inline int* GetNodes(int step) { int size = m_Dimension * m_Dimension + (int)std::pow(4, step); return new int[4]{ size, 30 , 15, 3 }; }
	inline glm::vec2 GetDirection(int index) { return m_Directions[index]; }
private:
	glm::vec2 m_Directions[4] = { {0,1}, {-1,0}, {0,-1}, {1,0} };

	int m_Dimension;
	int m_PlayerCount;
	int m_PlayerLeft;

	float m_GenerationData = 0.0f;
	int m_GenerationIndex = 0;
	int m_Step;

	Player** m_Players;
	std::vector<Can::Ref<Can::Texture2D>> m_Cards;
};