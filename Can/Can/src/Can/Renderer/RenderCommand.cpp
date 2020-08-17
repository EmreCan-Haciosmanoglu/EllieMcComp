#include "canpch.h"
#include "RenderCommand.h"

#include "Platform/OpenGl/OpenGLRendererAPI.h"

namespace Can
{
	Scope<RendererAPI> RenderCommand::s_RendererAPI = RendererAPI::Create();
}