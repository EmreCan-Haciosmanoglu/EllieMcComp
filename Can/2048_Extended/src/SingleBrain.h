#pragma once
#include "Can.h"
#include "../temps/NeuralNetwork.h"
#include "imgui.h"
#include "Puppet.h"

class SingleBrain
{
public:
	SingleBrain(int puppetCount, int dimension);
	~SingleBrain();

	inline void PuppetDied() { m_PuppetLeft--; }
	inline std::vector<Can::Ref<Can::Texture2D>> GetCards() { return m_Cards; }

	void DrawBestPuppet();
	void Tick();

	inline int GetPuppetCount() const { return m_PuppetCount; }
	inline int GetPuppetLeft() const { return m_PuppetLeft; }
	inline NeuralNetwork* GetTheBrain() { return m_TheBrain; }
	inline int* GetNodes() { int size = m_Dimension * m_Dimension; return new int[4]{ size, size / 2 , size / 4, 4 }; }
	inline glm::vec2 GetDirection(int index) { return m_Directions[index]; }
public:
	int maxScore = 0;
	int currentScore = 0;
	int Generation = 0;
private:
	glm::vec2 m_Directions[4] = { {0,1}, {-1,0}, {0,-1}, {1,0} };

	int m_Dimension;
	int m_PuppetCount;
	int m_PuppetLeft;

	Puppet** m_Puppets;
	NeuralNetwork* m_TheBrain;
	std::vector<Can::Ref<Can::Texture2D>> m_Cards;
};