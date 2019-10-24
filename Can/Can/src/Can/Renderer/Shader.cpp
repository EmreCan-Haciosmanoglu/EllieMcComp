#include "canpch.h"

#include "Renderer.h"

#include "Shader.h"
#include "Platform/OpenGl/OpenGLShader.h"

namespace Can
{
	Shader* Shader::Create(const std::string& vertexSrc, const std::string& fragmentSrc)
	{

		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: CAN_CORE_ASSERT(false, "RendererAPI::None is currently not supported!!"); return nullptr;
		case RendererAPI::API::OpenGL: return new OpenGLShader(vertexSrc, fragmentSrc);
		}
		CAN_CORE_ASSERT(false, "Unknown RendererAPI!!!");
		return nullptr;
	}
	Shader* Shader::Create(const std::string& filepath )
	{

		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: CAN_CORE_ASSERT(false, "RendererAPI::None is currently not supported!!"); return nullptr;
		case RendererAPI::API::OpenGL: return new OpenGLShader(filepath);
		}
		CAN_CORE_ASSERT(false, "Unknown RendererAPI!!!");
		return nullptr;
	}
}