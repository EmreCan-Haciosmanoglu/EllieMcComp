#pragma once
#include "Can.h"

class Sandbox3D;
class Scene : public Can::Layer::Layer
{
public:
	Scene(Sandbox3D* parent);
	virtual ~Scene() = default;

	virtual void OnAttach() override {}
	virtual void OnDetach() override {}

	virtual void OnUpdate(Can::TimeStep ts) override;
	virtual void OnEvent(Can::Event* event) override;

private:
	Sandbox3D* m_Parent;
	Can::Camera::Controller::Perspective m_MainCameraController;

};