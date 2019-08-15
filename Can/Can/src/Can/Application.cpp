#include "Application.h"

#include "Can/Events/ApplicationEvent.h"
#include "Can/Log.h"

namespace Can
{
	Application::Application()
	{

	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		Event::WindowResizeEvent e(1280, 720);
		CAN_TRACE(e);
		while (true)
		{
		}
	}
}