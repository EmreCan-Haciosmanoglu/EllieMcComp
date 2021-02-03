#include "canpch.h"
#include "Renderer2D.h"

#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Can
{
	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec4 TintColor;
		glm::vec2 TexCoord;
		float TextureIndex;
		float TilingFactor;
	};

	struct Renderer2DData
	{
		const uint32_t MaxQuads = 10000;
		const uint32_t MaxVertices = 10000 * 4;
		const uint32_t MaxIndices = 10000 * 6;
		static const uint32_t MaxTextureSlots = 32;

		Ref<VertexArray> QuadVertexArray;
		Ref<VertexBuffer> QuadVertexBuffer;
		Ref<Shader> TextureShader;
		Ref<Texture2D> WhiteTexture;

		uint32_t QuadIndexCount = 0;

		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1; // 0 => white texture;

		glm::vec4 QuadVertexPositions;
	};

	static Renderer2DData s_Data;

	void Renderer2D::Init()
	{
		CAN_PROFILE_FUNCTION();

		s_Data.QuadVertexArray = VertexArray::Create();

		s_Data.QuadVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertex));
		s_Data.QuadVertexBuffer->SetLayout({
			{ShaderDataType::Float3, "a_Position"},
			{ShaderDataType::Float4, "a_TintColor"},
			{ShaderDataType::Float2, "a_TexCoord"},
			{ShaderDataType::Float, "a_TexIndex"},
			{ShaderDataType::Float, "a_TilingFactor"}
			});
		s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);

		s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];

		uint32_t* quadIndices = new uint32_t[s_Data.MaxIndices];

		uint32_t offset = 0;
		for (size_t i = 0; i < s_Data.MaxIndices; i += 6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;
			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}
		Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, s_Data.MaxIndices);
		s_Data.QuadVertexArray->SetIndexBuffer(quadIB);
		delete[] quadIndices;


		s_Data.TextureShader = Shader::Create("assets/shaders/2DTexture.glsl");
		s_Data.WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data.WhiteTexture->SetData(&whiteTextureData, sizeof(whiteTextureData));

		int32_t samplers[s_Data.MaxTextureSlots];
		for (uint32_t i = 0; i < s_Data.MaxTextureSlots; i++)
			samplers[i] = i;

		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetIntArray("u_Textures", samplers, s_Data.MaxTextureSlots);

		s_Data.TextureSlots[0] = s_Data.WhiteTexture;

		s_Data.QuadVertexPositions = { -0.5f, -0.5f, 0.0f, 1.0f };
		//s_Data.QuadVertexPositions[1] = { +0.5f, -0.5f, 0.0f, 1.0f };
		//s_Data.QuadVertexPositions[2] = { +0.5f, +0.5f, 0.0f, 1.0f };
		//s_Data.QuadVertexPositions[3] = { -0.5f, +0.5f, 0.0f, 1.0f };
	}
	void Renderer2D::Shutdown()
	{
		CAN_PROFILE_FUNCTION();
	}

	void Renderer2D::BeginScene(const Camera::OrthographicCamera& camera)
	{
		CAN_PROFILE_FUNCTION();

		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
		
		s_Data.QuadIndexCount = 0;
		s_Data.TextureSlotIndex = 1;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
	}
	void Renderer2D::EndScene()
	{
		CAN_PROFILE_FUNCTION();

		uint32_t dataSize = (uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase;
		s_Data.QuadVertexBuffer->SetData((float*)s_Data.QuadVertexBufferBase, dataSize);

		Flush();
	}

	void Renderer2D::Flush()
	{
		CAN_PROFILE_FUNCTION();
		s_Data.QuadVertexArray->Bind();

		for (size_t i = 0; i < s_Data.TextureSlotIndex; i++)
			s_Data.TextureSlots[i]->Bind(i);
		RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);
	}

	void Renderer2D::DrawQuad(const DrawQuadParameters& parameters)
	{
		CAN_PROFILE_FUNCTION();

		glm::mat4 transform =
			glm::translate(glm::mat4(1.0f), parameters.Position)
			* glm::rotate(glm::mat4(1.0f), parameters.RotationInRadians, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), parameters.Size);

		DrawQuad(transform, parameters);
	}
	void Renderer2D::DrawQuad(const glm::mat4& transform, const DrawQuadParameters& parameters)
	{
		float textureIndex = 0.0f;
		if (parameters.texture != nullptr)
		{
			for (size_t i = 1; i < s_Data.TextureSlotIndex; i++)
			{
				if (*s_Data.TextureSlots[i].get() == *parameters.texture.get())
				{
					textureIndex = (float)i;
					break;
				}
			}
			if (textureIndex == 0.0f)
			{
				textureIndex = (float)s_Data.TextureSlotIndex;
				s_Data.TextureSlots[s_Data.TextureSlotIndex] = parameters.texture;
				s_Data.TextureSlotIndex++;
			}
		}
		glm::vec4 trim{ 1.0f, 1.0f, 0.0f, 0.0f };
		s_Data.QuadVertexBufferPtr->Position = transform * (s_Data.QuadVertexPositions + glm::vec4 { parameters.trim[3], 1.0f - parameters.trim[2], 0.0f, 0.0f });
		s_Data.QuadVertexBufferPtr->TintColor = parameters.TintColor;
		s_Data.QuadVertexBufferPtr->TexCoord = { parameters.trim[3], 1.0f - parameters.trim[2] };
		s_Data.QuadVertexBufferPtr->TextureIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = 1.0f;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * (s_Data.QuadVertexPositions + glm::vec4{ parameters.trim[1], 1.0f - parameters.trim[2], 0.0f, 0.0f });;
		s_Data.QuadVertexBufferPtr->TintColor = parameters.TintColor;
		s_Data.QuadVertexBufferPtr->TexCoord = { parameters.trim[1], 1.0f - parameters.trim[2] };
		s_Data.QuadVertexBufferPtr->TextureIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = 1.0f;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * (s_Data.QuadVertexPositions + glm::vec4{ parameters.trim[1], 1.0f - parameters.trim[0], 0.0f, 0.0f });
		s_Data.QuadVertexBufferPtr->TintColor = parameters.TintColor;
		s_Data.QuadVertexBufferPtr->TexCoord = { parameters.trim[1], 1.0f - parameters.trim[0] };
		s_Data.QuadVertexBufferPtr->TextureIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = 1.0f;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * (s_Data.QuadVertexPositions + glm::vec4{ parameters.trim[3], 1.0f - parameters.trim[0], 0.0f, 0.0f });
		s_Data.QuadVertexBufferPtr->TintColor = parameters.TintColor;
		s_Data.QuadVertexBufferPtr->TexCoord = { parameters.trim[3], 1.0f - parameters.trim[0] };
		s_Data.QuadVertexBufferPtr->TextureIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = 1.0f;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadIndexCount += 6;
	}
}