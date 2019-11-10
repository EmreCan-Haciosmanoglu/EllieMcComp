#include "canpch.h"
#include "OpenGLVertexArray.h"

#include <glad/glad.h>

namespace Can
{
	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
		case Can::ShaderDataType::Float: return GL_FLOAT;
		case Can::ShaderDataType::Float2:return GL_FLOAT;
		case Can::ShaderDataType::Float3:return GL_FLOAT;
		case Can::ShaderDataType::Float4:return GL_FLOAT;
		case Can::ShaderDataType::Mat3:return	GL_FLOAT;
		case Can::ShaderDataType::Mat4:return	GL_FLOAT;
		case Can::ShaderDataType::Int:return	GL_INT;
		case Can::ShaderDataType::Int2:return	GL_INT;
		case Can::ShaderDataType::Int3:return	GL_INT;
		case Can::ShaderDataType::Int4:return	GL_INT;
		case Can::ShaderDataType::Bool:return	GL_BOOL;
		}

		CAN_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}
	
	OpenGLVertexArray::OpenGLVertexArray()
	{
		glCreateVertexArrays(1, &m_RendererID);
	}
	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &m_RendererID);
	}
	
	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_RendererID);
	}
	void OpenGLVertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}
	
	void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexbuffer) 
	{
		CAN_CORE_ASSERT(vertexbuffer->GetLayout().GetElements().size(), "Vertex buffer has no layout");

		glBindVertexArray(m_RendererID);
		vertexbuffer->Bind();
		uint32_t index = 0;
		const auto& layout = vertexbuffer->GetLayout();
		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(index + m_VertexBufferIndexOffset);
			glVertexAttribPointer(
				index + m_VertexBufferIndexOffset,
				element.GetComponentCount(),
				ShaderDataTypeToOpenGLBaseType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)(intptr_t)element.Offset
			);
			index++;
		}
		m_VertexBuffers.push_back(vertexbuffer);
		m_VertexBufferIndexOffset += layout.GetElements().size();
	}
	void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexbuffer) 
	{
		glBindVertexArray(m_RendererID);
		indexbuffer->Bind();

		m_IndexBuffer = indexbuffer;
	}
}