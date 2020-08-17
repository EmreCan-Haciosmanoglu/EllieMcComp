#include "canpch.h"
#include "WindowsInput.h"

#include "Can/Application.h"
#include <GLFW/glfw3.h>

namespace Can
{
	Scope<Input> Input::s_Instance = CreateScope<Platform::Windows::WindowsInput>();
}

namespace Can::Platform::Windows
{

	bool WindowsInput::IsKeyPressedImp(KeyCode key)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetKey(window, static_cast<int32_t>(key));

		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}
	bool WindowsInput::IsMouseButtonPressedImp(MouseCode button)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetMouseButton(window, static_cast<int32_t>(button));

		return state == GLFW_PRESS;
	}
	float WindowsInput::GetMouseXImp()
	{
		auto[x, y] = GetMousePosImp();
		return x;
	}
	float WindowsInput::GetMouseYImp()
	{
		auto[x, y] = GetMousePosImp();
		return y;
	}
	std::pair<float, float> WindowsInput::GetMousePosImp()
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double x, y;
		glfwGetCursorPos(window, &x, &y);
		return { x, y };
	}
}