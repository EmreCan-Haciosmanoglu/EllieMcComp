#include "Can.h"

class Sandbox : public Can::Application
{
public:
	Sandbox()
	{

	}

	~Sandbox()
	{

	}
};

Can::Application* Can::CreateApplication()
{
	return new Sandbox();
}