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
	f32 WindowsInput::GetMouseXImp()
	{
		auto[x, y] = get_mouse_position_imp_float();
		return x;
	}
	f32 WindowsInput::GetMouseYImp()
	{
		auto[x, y] = get_mouse_position_imp_float();
		return y;
	}
	std::pair<u32, u32> WindowsInput::get_mouse_position_imp()
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double x, y;
		glfwGetCursorPos(window, &x, &y);
		return { (u32)x, (u32)y };
	}
	std::pair<f32, f32> WindowsInput::get_mouse_position_imp_float()
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double x, y;
		glfwGetCursorPos(window, &x, &y);
		return { x, y };
	}
}