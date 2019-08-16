#pragma once

#include "Core.h"
#include "Can/Events/Event.h"
#include "Window.h"

namespace Can
{
	class CAN_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	private:
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
	};

	Application* CreateApplication();
}