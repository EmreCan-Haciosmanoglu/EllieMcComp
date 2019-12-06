#pragma once
#include "Can.h"
#include "Play.h"

class Game {
public:
	Game(unsigned int playCount);
	~Game();

	void DrawBestPlay();
	void Tick();

public:
	glm::vec4 m_RedColor = { 1.0f, 0.0f, 0.0f, 1.0f };
	glm::vec4 m_WhitishColor = { 0.9f, 0.9f, 0.9f, 1.0f };
	glm::vec2 m_Directions[4] = { {0,1}, {1,0}, {0,-1}, {-1,0} };
private:
	glm::vec2 m_Offset = { -7.0f, +3.0f };
	unsigned int m_PlayCount;
	unsigned int m_PlayLeft;
	Play** m_Plays;
};