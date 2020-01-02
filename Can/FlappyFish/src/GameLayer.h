#pragma once
#include "Can.h"
#include "imgui.h"

#include "Scene.h"
#include "Obstacle.h"
#include "Fish.h"

class GameLayer : public Can::Layer::Layer
{
public:
	GameLayer();
	virtual ~GameLayer() = default;

	virtual void OnAttach() override {}
	virtual void OnDetach() override {}

	virtual void OnUpdate(Can::TimeStep ts) override;
	virtual void OnEvent(Can::Event::Event& event) override;
	virtual void OnImGuiRender() override;

	void FishIsDied() { m_FishLeft--; }
	void AddPoint();
private:
	void UpdateGame(float ts);
	void DrawGame(float height);
public:
	Can::Ref<Can::Texture2D> m_FishImage = Can::Texture2D::Create("assets/textures/Fish.png");
	Can::Ref<Can::Texture2D> m_BackgroundImage = Can::Texture2D::Create("assets/textures/Background.png");
	Can::Ref<Can::Texture2D> m_WaterImage = Can::Texture2D::Create("assets/textures/Water.png");
	Can::Ref<Can::Texture2D> m_StalactiteImage = Can::Texture2D::Create("assets/textures/Stalactite.png");
private:
	Can::Camera::OrthographicCameraController m_CameraController;

	int m_FishCount;
	int m_FishLeft;

	int m_MaxPoint = 0;
	int m_CurrentMaxPoint = 0;
	int m_Generation = 0;

	Scene* m_Scene;
	Fish** m_Fish;

};