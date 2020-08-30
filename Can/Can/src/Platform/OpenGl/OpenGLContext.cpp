#include "canpch.h"

#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Can
{
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		:m_WindowHandle(windowHandle)
	{
		CAN_PROFILE_FUNCTION();

		CAN_CORE_ASSERT(windowHandle, "Window handle is null!");
	}
	void OpenGLContext::Init()
	{
		CAN_PROFILE_FUNCTION();

		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		CAN_CORE_ASSERT(status, "Failed to initialize Glad!");

		CAN_CORE_INFO(
			"OpenGL Info---\n      Vendor: {0}\n      Renderer: {1}\n      Version: {2}\n      Shading Version: {3}",
			glGetString(GL_VENDOR),
			glGetString(GL_RENDERER),
			glGetString(GL_VERSION),
			glGetString(GL_SHADING_LANGUAGE_VERSION)
		);

#ifdef CAN_ENABLE_ASSERTS
		int versionMajor;
		int versionMinor;
		glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
		glGetIntegerv(GL_MINOR_VERSION, &versionMinor);

		CAN_CORE_ASSERT(versionMajor > 4 || (versionMajor == 4 && versionMinor >= 5), "Can requires at least OpenGL version 4.5!");
#endif
	}

	void OpenGLContext::SwapBuffers()
	{
		CAN_PROFILE_FUNCTION();

		glfwSwapBuffers(m_WindowHandle);
	}
}