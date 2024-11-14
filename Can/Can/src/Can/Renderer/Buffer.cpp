#include "canpch.h"
#include "Buffer.h"

#include "OpenGLRenderer.h"
#include "Platform/OpenGl/OpenGLBuffer.h"
namespace Can
{
	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size, bool isDynamic)
	{
		switch (OpenGLRenderer::GetAPI())
		{
		case RendererAPI::API::None: assert(false && "RendererAPI::None is currently not supported!!"); return nullptr;
		case RendererAPI::API::OpenGL: return CreateRef< OpenGLVertexBuffer>(vertices, size, isDynamic);
		}
		assert(false && "Unknown RendererAPI!!!");
		return nullptr;
	}
	Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
	{
		{
		case RendererAPI::API::None: assert(false && "RendererAPI::None is currently not supported!!"); return nullptr;
		case RendererAPI::API::OpenGL: return CreateRef<OpenGLVertexBuffer>(size);
		}
		assert(false && "Unknown RendererAPI!!!");
		return nullptr;
	}
	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		switch (OpenGLRenderer::GetAPI())
		{
		case RendererAPI::API::None: assert(false && "RendererAPI::None is currently not supported!!"); return nullptr;
		case RendererAPI::API::OpenGL: return CreateRef<OpenGLIndexBuffer>(indices, count);
		}
		assert(false && "Unknown RendererAPI!!!");
		return nullptr;
	}
}