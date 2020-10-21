#include "canpch.h"
#include "Framebuffer.h"

#include "Renderer.h"
#include "Platform/OpenGl/OpenGLFramebuffer.h"

namespace Can
{
	Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec)
	{
		CAN_PROFILE_FUNCTION();

		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None: CAN_CORE_ASSERT(false, "RendererAPI::None is currently not supported!!"); return nullptr;
			case RendererAPI::API::OpenGL: return CreateRef<OpenGLFramebuffer>(spec);
		}
		CAN_CORE_ASSERT(false, "Unknown RendererAPI!!!");
		return nullptr;
	}
}