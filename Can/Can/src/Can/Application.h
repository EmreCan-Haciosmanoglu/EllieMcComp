#pragma once

#include "Core.h"

#include "Window.h"
#include "Can/Events/Event.h"
#include "Can/Layers/LayerStack.h"
#include "Can/Events/ApplicationEvent.h"


namespace Can
{
	class CAN_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Can::Event::Event& e);

		void PushLayer(Layer::Layer* layer);
		void PushOverlay(Layer::Layer* layer);
	private:
		bool OnWindowClose(Event::WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		Layer::LayerStack m_LayerStack;
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}