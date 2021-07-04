#pragma once
#include "Can/Renderer/Texture.h"

namespace Can
{
	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(uint32_t width, uint32_t height);
		OpenGLTexture2D(const std::string& path);
		OpenGLTexture2D(uint32_t id, uint32_t width, uint32_t height, const std::string& path);
		virtual ~OpenGLTexture2D();

		virtual void SetData(void* data, uint32_t size) override;
		virtual void SetSubData(void* data, uint32_t xoffset, uint32_t yoffset, uint32_t width, uint32_t height) override;

		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual uint32_t GetHeight() const override { return m_Height; }

		virtual void Bind(uint32_t slot = 0) const override;
		virtual bool operator==(const Texture& other) const override
		{
			return m_RendererID == ((OpenGLTexture2D&)other).m_RendererID;
		}

	private:
		std::string m_Path;

		uint32_t m_Width;
		uint32_t m_Height;

		uint32_t m_RendererID;

		unsigned int m_InternalFormat;
		unsigned int m_DataFormat;
	};
}