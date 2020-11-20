#include "canpch.h"
#include "ShadowFrameBuffer.h"
#include "Can/Application.h"
#include "Can/Renderer/RenderCommand.h"
#include <glad/glad.h>
namespace Can
{
	ShadowFrameBuffer::ShadowFrameBuffer(int width, int height)
		: width(width)
		, height(height)
	{
		Init();
	}

	ShadowFrameBuffer::~ShadowFrameBuffer()
	{
		glDeleteFramebuffers(1, &fbo);
		glDeleteTextures(1, &shadowMap);
	}

	void ShadowFrameBuffer::Init()
	{
		// configure depth map FBO
		glGenFramebuffers(1, &fbo);

		// create depth texture
		glGenTextures(1, &shadowMap);
		glBindTexture(GL_TEXTURE_2D, shadowMap);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

		// attach depth texture as FBO's depth buffer
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMap, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

	}
	void ShadowFrameBuffer::Bind()
	{
		RenderCommand::SetViewport(0, 0, width, height);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	}
	void ShadowFrameBuffer::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		Window& window = Application::Get().GetWindow();
		RenderCommand::SetViewport(0, 0, window.GetWidth(), window.GetHeight());
	}
}