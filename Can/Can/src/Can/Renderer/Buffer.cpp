#include "canpch.h"
#include "Buffer.h"

#include "Renderer.h"
#include "Platform/OpenGl/OpenGLBuffer.h"
namespace Can
{
	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size, bool isDynamic)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: CAN_CORE_ASSERT(false, "RendererAPI::None is currently not supported!!"); return nullptr;
		case RendererAPI::API::OpenGL: return std::make_shared < OpenGLVertexBuffer>(vertices, size, isDynamic);
		}
		CAN_CORE_ASSERT(false, "Unknown RendererAPI!!!");
		return nullptr;
	}
	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: CAN_CORE_ASSERT(false, "RendererAPI::None is currently not supported!!"); return nullptr;
		case RendererAPI::API::OpenGL: return std::make_shared<OpenGLIndexBuffer>(indices, size);
		}
		CAN_CORE_ASSERT(false, "Unknown RendererAPI!!!");
		return nullptr;
	}
}