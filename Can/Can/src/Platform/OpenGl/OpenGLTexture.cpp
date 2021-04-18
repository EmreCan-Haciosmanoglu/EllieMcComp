#include "canpch.h"
#include "OpenGLTexture.h"

#include "stb_image.h"
#include <glad/glad.h>

namespace Can
{
	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height)
		: m_Width(width)
		, m_Height(height)
		, m_InternalFormat(GL_RGBA8)
		, m_DataFormat(GL_RGBA)
	{
		CAN_PROFILE_FUNCTION();

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}

	OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
		:m_Path(path)
	{
		CAN_PROFILE_FUNCTION();

		int width, height, channels;

		stbi_set_flip_vertically_on_load(1);

		stbi_uc* data = nullptr;
		{
			CAN_PROFILE_SCOPE("stbi_load - OpenGLTexture2D::OpenGLTexture2D(const std:string&)");

			data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		}
		CAN_CORE_ASSERT(data, "Failed to load image!");

		m_Width = width;
		m_Height = height;

		if (channels == 4)
		{
			m_InternalFormat = GL_RGBA8;
			m_DataFormat = GL_RGBA;
		}
		else if (channels == 3)
		{
			m_InternalFormat = GL_RGB8;
			m_DataFormat = GL_RGB;
		}
		CAN_CORE_ASSERT(m_InternalFormat & m_DataFormat, "Format isn't supported!");

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}

	OpenGLTexture2D::OpenGLTexture2D(uint32_t id, uint32_t width, uint32_t height, const std::string& path)
		: m_RendererID(id)
		, m_Width(width)
		, m_Height(height)
		, m_Path("")
	{

		// REDO:
		m_InternalFormat = GL_RGBA8;
		m_DataFormat = GL_RGBA;
	}
	OpenGLTexture2D::~OpenGLTexture2D()
	{
		CAN_PROFILE_FUNCTION();

		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTexture2D::SetData(void* data, uint32_t size)
	{
		CAN_PROFILE_FUNCTION();

#ifdef CAN_ENABLE_ASSERTS
		uint32_t bpp = m_DataFormat == GL_RGBA ? 4 : 3;
		CAN_CORE_ASSERT(size == m_Width * m_Height * bpp, "Data must be entire texture!");
#endif

		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
	}

	void OpenGLTexture2D::SetSubData(void* data, uint32_t xoffset, uint32_t yoffset, uint32_t width, uint32_t height)
	{
		glTextureSubImage2D(m_RendererID, 0, xoffset, yoffset, width, height, m_DataFormat, GL_UNSIGNED_BYTE, data);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		CAN_PROFILE_FUNCTION();

		glBindTextureUnit(slot, m_RendererID);
	}
}