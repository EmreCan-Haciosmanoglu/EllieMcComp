#include "canpch.h"
#include "Can/Random.h"
#include "Can/Noise/Perlin.h"

#include "Application.h"
#include "Can/Renderer/Renderer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Can
{

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		CAN_PROFILE_FUNCTION();

		CAN_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		Utility::Random rand;
		m_Window = Window::Create(WindowProps{ "Sky Is The Limit v0.3-alpha", 1600, 900 });
		m_Window->SetEventCallback(CAN_BIND_EVENT_FN(Application::OnEvent));

		Renderer::Init();

		m_ImGuiLayer = new Layer::ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	Application::~Application()
	{
		CAN_PROFILE_FUNCTION();

		Renderer::Shutdown();
	}

	void Application::PushLayer(Layer::Layer* layer)
	{
		CAN_PROFILE_FUNCTION();

		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer::Layer* overlay)
	{
		CAN_PROFILE_FUNCTION();

		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	void Application::OnEvent(Event::Event& e)
	{
		CAN_PROFILE_FUNCTION();

		Event::EventDispatcher dispatcher(e);
		dispatcher.Dispatch<Event::WindowCloseEvent>(CAN_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<Event::WindowResizeEvent>(CAN_BIND_EVENT_FN(Application::OnWindowResize));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->OnEvent(e);
			if (e.m_Handled)
				break;
		}
	}

	void Application::Run()
	{
		CAN_PROFILE_FUNCTION();

		while (m_Running)
		{
			CAN_PROFILE_SCOPE("RunLoop");
			float time = (float)glfwGetTime();
			TimeStep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			if (!m_Minimized)
			{
				{
					CAN_PROFILE_SCOPE("LayerStack OnUpdate");

					for (Layer::Layer* layer : m_LayerStack)
						layer->OnUpdate(timestep);
				}

				m_ImGuiLayer->Begin();
				{
					CAN_PROFILE_SCOPE("LayerStack OnImGuiRender");

					for (Layer::Layer* layer : m_LayerStack)
						layer->OnImGuiRender();
				}
				m_ImGuiLayer->End();
			}


			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(Event::WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	bool Application::OnWindowResize(Event::WindowResizeEvent& e)
	{
		CAN_PROFILE_FUNCTION();

		if (e.GetHeight() == 0 || e.GetWidth() == 0)
		{
			m_Minimized = true;
			return false;
		}
		m_Minimized = false;

		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

		return false;
	}
}