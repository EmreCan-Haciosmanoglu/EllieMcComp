#pragma once
#include "Can.h"
#include "imgui.h"

#include "Scene.h"
#include "Obstacle.h"
#include "Dinosaur.h"

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

	void DinosaurIsDied() { m_DinosaurLeft--; }
	void AddPoint();
private:
	void UpdateGame(float ts);
	void DrawGame();
public:
	Can::Ref<Can::Texture2D> m_DinosaurImage = Can::Texture2D::Create("assets/textures/Dinosaur.png");
	Can::Ref<Can::Texture2D> m_SkyImage = Can::Texture2D::Create("assets/textures/Sky.png");
	Can::Ref<Can::Texture2D> m_GroundImage = Can::Texture2D::Create("assets/textures/Ground.png");
	Can::Ref<Can::Texture2D> m_CactusImage1 = Can::Texture2D::Create("assets/textures/Cactus1.png");
	Can::Ref<Can::Texture2D> m_CactusImage2 = Can::Texture2D::Create("assets/textures/Cactus2.png");
private:
	Can::Camera::OrthographicCameraController m_CameraController;

	int m_DinosaurCount;
	int m_DinosaurLeft;

	int m_MaxPoint = 0;
	int m_CurrentMaxPoint = 0;
	int m_Generation = 0;
	float m_CurrentSpeed = 1.5f;

	Scene* m_Scene;
	Dinosaur** m_Dinosaurs;

};