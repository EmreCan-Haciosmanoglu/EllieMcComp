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

	Application::Application(const WindowProps& props)
	{
		CAN_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		Random rand{};
		m_Window = Window::Create(props);
		m_Window->SetEventCallback(CAN_BIND_EVENT_FN(Application::OnEvent));

		Renderer::Init();
	}

	Application::~Application()
	{
		Renderer::Shutdown();
	}

	void Application::PushLayer(Layer::Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer::Layer* overlay)
	{
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	void Application::PopLayer(Layer::Layer* layer)
	{
		m_LayerStack.PopLayer(layer);
	}

	void Application::PopOverlay(Layer::Layer* overlay)
	{
		m_LayerStack.PopOverlay(overlay);
	}

	void Application::OnEvent(Event* e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(CAN_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(CAN_BIND_EVENT_FN(Application::OnWindowResize));
		if (e->m_Handled)
		{
			delete e;
			return;
		}

		if (e->GetEventType() == MouseButtonReleasedEvent::GetStaticType())
		{
			events.push_back(e);
			return;
		}

		for (auto it{ m_LayerStack.end() }; it != m_LayerStack.begin();)
		{
			(*--it)->OnEvent(e);
			if (e->m_Handled)
			{
				delete e;
				break;
			}
		}
	}

	void Application::Run()
	{
		while (m_Running)
		{
			f32 time{ (f32)glfwGetTime() };
			TimeStep timestep{ time - m_LastFrameTime };
			m_LastFrameTime = time;

			if (!m_Minimized)
			{
				for (auto it{ m_LayerStack.end() }; it != m_LayerStack.begin();)
				{
					auto layer{ (*--it) };
					bool force_update{ layer->OnUpdate(timestep) };
					if (force_update) goto end;
					for (u64 i{ 0 }; i < events.size(); ++i)
					{
						auto e{ events[i] };
						layer->OnEvent(e);
						e->seen = true;
						if (e->m_Handled)
						{
							events.erase(events.begin() + i);
							--i;
							delete e;
						}
					}
				}
				for (u64 i{ events.size() }; i > 0; --i)
				{
					auto e{ events[i - 1] };
					if (e->seen)
					{
						events.erase(events.begin() + i - 1);
						delete e;
					}
				}
			}


			m_Window->OnUpdate();
		end:
			int a = 1;
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent* e)
	{
		m_Running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent* e)
	{
		if (e->height == 0 || e->width == 0)
		{
			m_Minimized = true;
			return false;
		}
		m_Minimized = false;

		Renderer::OnWindowResize(e->width, e->height);

		return false;
	}
}