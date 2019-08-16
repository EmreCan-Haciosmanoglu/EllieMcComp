#pragma once

#include "Core.h"
#include "Can/Events/Event.h"
#include "Can/Events/ApplicationEvent.h"

#include "Window.h"

namespace Can
{
	class CAN_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Can::Event::Event& e);
	private:
		bool OnWindowClose(Event::WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
	};

	Application* CreateApplication();
}