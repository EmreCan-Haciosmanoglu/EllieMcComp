#pragma once
#include "Can.h"
#include "Obstacle.h"

class GameLayer;
class Scene
{
public:
	Scene(float width, float heigth, GameLayer* gameLayer);
	~Scene();

	void Update(float ts);
	void Draw();

	inline Obstacle GetObstacle(int i) { return m_Obstacles.at(i); }

private:
	void AddObstacle();
private:
	int m_ObstacleCount;

	float m_Width;
	float m_Height;

	float m_MovementSpeed;

	float m_ObstacleWidth;

	float m_MinObstacleHeight;
	float m_MaxObstacleHeight;

	float m_MinSpaceHeight;
	float m_MaxSpaceHeight;

	float m_MinSpaceWidth;
	float m_MaxSpaceWidth;

	std::vector<Obstacle> m_Obstacles;

	Can::Ref<Can::Texture2D> m_ObstacleTexture;
	GameLayer* m_GameLayer;
};