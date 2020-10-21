#include "canpch.h"
#include "Texture.h"

#include "Renderer.h"
#include "Platform/OpenGl/OpenGLTexture.h"

namespace Can
{
	Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
	{
		CAN_PROFILE_FUNCTION();

		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: CAN_CORE_ASSERT(false, "RendererAPI::None is currently not supported!!"); return nullptr;
		case RendererAPI::API::OpenGL: return CreateRef<OpenGLTexture2D>(width, height);
		}
		CAN_CORE_ASSERT(false, "Unknown RendererAPI!!!");
		return nullptr;
	}
	Ref<Texture2D> Texture2D::Create(const std::string& path)
	{
		CAN_PROFILE_FUNCTION();

		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: CAN_CORE_ASSERT(false, "RendererAPI::None is currently not supported!!"); return nullptr;
		case RendererAPI::API::OpenGL: return CreateRef<OpenGLTexture2D>(path);
		}
		CAN_CORE_ASSERT(false, "Unknown RendererAPI!!!");
		return nullptr;
	}
	Ref<Texture2D> Texture2D::Create(uint32_t id, uint32_t width, uint32_t height, const std::string& path)
	{
		CAN_PROFILE_FUNCTION();

		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: CAN_CORE_ASSERT(false, "RendererAPI::None is currently not supported!!"); return nullptr;
		case RendererAPI::API::OpenGL: return CreateRef<OpenGLTexture2D>(id, width, height, path);
		}
		CAN_CORE_ASSERT(false, "Unknown RendererAPI!!!");
		return nullptr;
	}
}