#include "canpch.h"
#include "VertexArray.h"
#include "Platform/OpenGl/OpenGLVertexArray.h"

#include "OpenGLRenderer.h"
namespace Can
{
	Ref<VertexArray> VertexArray::Create()
	{
		switch (OpenGLRenderer::GetAPI())
		{
		case RendererAPI::API::None: assert(false && "RendererAPI::None is currently not supported!!"); return nullptr;
		case RendererAPI::API::OpenGL: return CreateRef<OpenGLVertexArray>();
		}
		assert(false && "Unknown RendererAPI!!!");
		return nullptr;
	}
}