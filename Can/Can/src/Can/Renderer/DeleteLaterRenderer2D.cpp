#include "canpch.h"
#include "DeleteLaterRenderer2D.h"

namespace Can
{
	DeleteLaterRenderer2DStorage* DeleteLaterRenderer2D::s_Data = nullptr;

	void DeleteLaterRenderer2D::Init()
	{
		s_Data = new DeleteLaterRenderer2DStorage();
		s_Data->QuadVertexArray = VertexArray::Create();

		float vertices[4 * (3 + 2)] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};
		Ref<VertexBuffer> squareVB;
		squareVB = VertexBuffer::Create(vertices, sizeof(vertices));
		squareVB->SetLayout({
			{ShaderDataType::Float3, "a_Position"},
			{ShaderDataType::Float2, "a_TexCoord"}
			});
		s_Data->QuadVertexArray->AddVertexBuffer(squareVB);

		uint32_t indices[6]{ 0,1,2,2,3,0 };
		Ref<IndexBuffer> squareIB;
		squareIB = IndexBuffer::Create(indices, 6);
		s_Data->QuadVertexArray->SetIndexBuffer(squareIB);

		s_Data->TextureShader = Shader::Create("assets/shaders/2DTexture.glsl");
		s_Data->WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data->WhiteTexture->SetData(&whiteTextureData, sizeof(whiteTextureData));

		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetInt("u_Texture", 0);
	}
	void DeleteLaterRenderer2D::Shutdown()
	{
		delete s_Data;
	}

	void DeleteLaterRenderer2D::BeginScene(const Camera::OrthographicCamera& camera)
	{
		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
	}
	void DeleteLaterRenderer2D::EndScene()
	{
	}

	void DeleteLaterRenderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color)
	{
		DrawQuad(transform, s_Data->WhiteTexture, color);
	}
	void DeleteLaterRenderer2D::DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, const glm::vec4& tintColor)
	{
		if (texture)
			texture->Bind();

		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);

	}

	void DeleteLaterRenderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}
	void DeleteLaterRenderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad(position, size, s_Data->WhiteTexture, color);
	}

	void DeleteLaterRenderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		DrawQuad({ position.x,position.y,0.0f }, size, rotation, color);
	}
	void DeleteLaterRenderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		DrawQuad(position, size, rotation, s_Data->WhiteTexture, color);
	}


	void DeleteLaterRenderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& tintColor)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture, tintColor);
	}
	void DeleteLaterRenderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& tintColor)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x,size.y,1.0f });
		DrawQuad(transform, texture, tintColor);
	}

	void DeleteLaterRenderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, const glm::vec4& tintColor)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, rotation, texture);
	}
	void DeleteLaterRenderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, const glm::vec4& tintColor)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0, 0, 1)) * glm::scale(glm::mat4(1.0f), { size.x,size.y,1.0f });
		DrawQuad(transform, texture, tintColor);
	}
}