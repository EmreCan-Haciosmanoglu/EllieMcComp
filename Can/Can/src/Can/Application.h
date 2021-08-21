#pragma once

#include "Core.h"

#include "Window.h"

#include "Can/Events/Event.h"
#include "Can/Events/ApplicationEvent.h"

#include "Can/Layers/LayerStack.h"
#include "Can/Layers/ImGuiLayer.h"

#include "Can/Core/TimeStep.h"

#include "Can/Renderer/Shader.h"
#include "Can/Renderer/VertexArray.h"

namespace Can
{
	class  Application
	{
	public:
		Application(const WindowProps& props);
		virtual ~Application();

		void Run();

		void OnEvent(Can::Event::Event& e);

		void PushLayer(Layer::Layer* layer);
		void PushOverlay(Layer::Layer* overlay);

		void PopLayer(Layer::Layer* layer);
		void PopOverlay(Layer::Layer* overlay);

		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }
	private:
		bool OnWindowClose(Event::WindowCloseEvent& e);
		bool OnWindowResize(Event::WindowResizeEvent& e);

	public:
		bool m_Running = true;
	private:
		float m_LastFrameTime = 0.0f;
		Scope<Window> m_Window;
		Layer::ImGuiLayer* m_ImGuiLayer;
		bool m_Minimized = false;
		Layer::LayerStack m_LayerStack;

		static Application* s_Instance;
	};

	Application* CreateApplication(const Can::WindowProps& props);
}