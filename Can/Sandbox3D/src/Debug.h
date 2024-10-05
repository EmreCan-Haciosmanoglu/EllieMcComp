#pragma once
#include "Can.h"
#include "imgui.h"

class Sandbox3D;
class Debug : public Can::Layer::Layer
{
public:
	Debug(Sandbox3D* parent);
	virtual ~Debug() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnUpdate(Can::TimeStep ts) override;
	virtual void OnEvent(Can::Event* event) override;
	virtual void OnImGuiRender() override;
private:
	Sandbox3D* m_Parent;
};