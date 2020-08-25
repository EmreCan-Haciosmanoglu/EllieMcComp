#pragma once
#include "Can/Renderer/Buffer.h"

namespace Can
{
	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(float* vertices, uint32_t size, bool isDynamic = false);
		OpenGLVertexBuffer(uint32_t size);
		virtual ~OpenGLVertexBuffer();

		virtual void ReDo(float* vertices, uint32_t size) override;
		virtual void Resize(uint32_t size) override;
		virtual void SetData(const void* data, uint32_t size) override;
		virtual void SetSubData(float* vertices, int start, int offset) override;

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual inline const BufferLayout& GetLayout() const override { return m_Layout; }
		virtual inline void SetLayout(const BufferLayout& layout) override { m_Layout = layout; }
	private:
		uint32_t m_RendererID;
		BufferLayout m_Layout;
	};

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
		virtual ~OpenGLIndexBuffer();

		virtual void ReDo(uint32_t* indices, uint32_t count) override;

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual uint32_t GetCount() const override { return m_Count; }
	private:
		uint32_t m_RendererID;
		uint32_t m_Count;
	};
}