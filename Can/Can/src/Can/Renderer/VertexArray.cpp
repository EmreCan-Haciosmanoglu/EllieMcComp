#include "canpch.h"
#include "VertexArray.h"
#include "Platform/OpenGl/OpenGLVertexArray.h"

#include "Renderer.h"
namespace Can
{
	VertexArray* VertexArray::Create()
	{

		switch (Renderer::GetAPI())
		{
		case RendererAPI::None: CAN_CORE_ASSERT(false, "RendererAPI::None is currently not supported!!"); return nullptr;
		case RendererAPI::OpenGL: return new OpenGLVertexArray();
		}
		CAN_CORE_ASSERT(false, "Unknown RendererAPI!!!");
		return nullptr;
	}
}