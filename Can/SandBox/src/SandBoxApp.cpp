#include "canpch.h"
#include "Can.h"

class ExampleLayer : public Can::Layer::Layer
{
public:
	ExampleLayer()
		:Can::Layer::Layer("Example")
	{
	}

	void OnUpdate() override
	{
		if (Can::Input::IsKeyPressed(CAN_KEY_F))
			CAN_CORE_WARN("F key is pressed!!!");
	}

	void OnEvent(Can::Event::Event& event) override
	{
		//CAN_TRACE("{0}", event);
	}
};

class Sandbox : public Can::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
		PushOverlay(new Can::Layer::ImGuiLayer());
	}

	~Sandbox()
	{

	}
};

Can::Application* Can::CreateApplication()
{
	return new Sandbox();
}