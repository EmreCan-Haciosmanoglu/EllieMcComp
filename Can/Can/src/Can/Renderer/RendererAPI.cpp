#include "canpch.h"
#include "RendererAPI.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Can
{
	RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;

	Scope<RendererAPI> RendererAPI::Create()
	{
		CAN_PROFILE_FUNCTION();

		switch (s_API)
		{
		case RendererAPI::API::None:    CAN_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return CreateScope<OpenGLRendererAPI>();
		}

		CAN_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}