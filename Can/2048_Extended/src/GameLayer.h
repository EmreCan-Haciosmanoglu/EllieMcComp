#pragma once
#include "Can.h"

#include "imgui.h"
#include "Can/Renderer/Texture.h"
#include "Generation.h"
#include "SingleBrain.h"

class GameLayer : public Can::Layer::Layer
{
public:
	GameLayer();
	virtual ~GameLayer() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnUpdate(Can::TimeStep ts) override;
	virtual void OnEvent(Can::Event::Event& event) override;
	virtual void OnImGuiRender() override;
private:
	Can::Camera::OrthographicCameraController m_CameraController;
	Generation* m_Generation;
	SingleBrain* m_Game;
	int max = 0;
};