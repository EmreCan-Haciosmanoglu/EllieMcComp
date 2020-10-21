#pragma once

#include "Can/Renderer/Framebuffer.h"

namespace Can
{
	class OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer(const FramebufferSpecification& spec);
		virtual ~OpenGLFramebuffer();

		void Invalidate();

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual const FramebufferSpecification& GetSpecification() const override { return m_Specification; }
		
		virtual uint32_t GetColorAttachmentRendererID() const override { return m_ColorAttachment;}
		virtual uint32_t GetDepthAttachmentRendererID() const override { return m_DepthAttachment;}

	private:
		uint32_t m_RendererID;
		uint32_t m_ColorAttachment, m_DepthAttachment;
		FramebufferSpecification m_Specification;
	};
}