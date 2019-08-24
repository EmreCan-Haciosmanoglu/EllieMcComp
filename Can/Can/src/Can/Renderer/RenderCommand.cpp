#include "canpch.h"
#include "RenderCommand.h"

#include "Platform/OpenGl/OpenGLRendererAPI.h"

namespace Can
{
	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;
}