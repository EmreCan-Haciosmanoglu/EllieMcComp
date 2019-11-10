#pragma once

#include "Can/Renderer/VertexArray.h"

namespace Can
{
	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexbuffer)  override;
		virtual void SetIndexBuffer(const Ref<IndexBuffer>& indexbuffer)  override;

		virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const override { return m_VertexBuffers; }
		virtual const Ref<IndexBuffer>& GetIndexBuffer() const override { return m_IndexBuffer; };
	private:
		uint32_t m_RendererID;
		uint32_t m_VertexBufferIndexOffset = 0;

		std::vector<Ref<VertexBuffer>> m_VertexBuffers;
		Ref<IndexBuffer> m_IndexBuffer;
	};
}