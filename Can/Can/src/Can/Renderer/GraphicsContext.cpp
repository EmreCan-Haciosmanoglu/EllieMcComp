#include "canpch.h"
#include "GraphicsContext.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLContext.h"

namespace Can {

	Scope<GraphicsContext> GraphicsContext::Create(void* window)
	{
		CAN_PROFILE_FUNCTION();

		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    CAN_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return CreateScope<OpenGLContext>(static_cast<GLFWwindow*>(window));
		}

		CAN_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}