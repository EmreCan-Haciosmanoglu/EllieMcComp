#include "canpch.h"
#include "GraphicsContext.h"

#include "OpenGLRenderer.h"
#include "Platform/OpenGL/OpenGLContext.h"

namespace Can {

	Scope<GraphicsContext> GraphicsContext::Create(void* window)
	{
		switch (OpenGLRenderer::GetAPI())
		{
		case RendererAPI::API::None:    assert(false && "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return CreateScope<OpenGLContext>(static_cast<GLFWwindow*>(window));
		}

		assert(false && "Unknown RendererAPI!");
		return nullptr;
	}

}