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
		CAN_INFO("ExampleLayer::Update");
	}

	void OnEvent(Can::Event::Event& event) override
	{
		CAN_TRACE("{0}", event);
	}
};

class Sandbox : public Can::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}

	~Sandbox()
	{

	}
};

Can::Application* Can::CreateApplication()
{
	return new Sandbox();
}