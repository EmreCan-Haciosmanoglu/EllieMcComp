#pragma once

#include "Core.h"

#include "Window.h"
#include "Can/Events/Event.h"
#include "Can/Events/ApplicationEvent.h"
#include "Can/Layers/LayerStack.h"
#include "Can/Layers/ImGuiLayer.h"
#include "Can/Renderer/Shader.h"
#include "Can/Renderer/OrthographicCamera.h"

#include "Platform/OpenGl/OpenGLBuffer.h"
#include "Platform/OpenGl/OpenGLVertexArray.h"

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

		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }
	private:
		bool OnWindowClose(Event::WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		Layer::ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		Layer::LayerStack m_LayerStack;

		std::shared_ptr<Shader> m_Shader;
		std::shared_ptr <VertexArray> m_VertexArray;

		OrthographicCamera m_Camera;

		static Application* s_Instance;
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}