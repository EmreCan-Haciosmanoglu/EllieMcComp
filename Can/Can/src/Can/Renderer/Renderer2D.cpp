#include "canpch.h"
#include "Renderer2D.h"

#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"
#include "Can/Font/Font.h"
#include "Can/Font/FontAtlas.h"
#include <glm/gtc/matrix_transform.hpp>

#include "ft2build.h"
#include FT_FREETYPE_H

#include "Can/Font/FontFlags.h"

namespace Can
{
	struct QuadVertex
	{
		v3 Position;
		v4 TintColor;
		v2 TexCoord;
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

		uint32_t QuadIndexCount{ 0 };

		QuadVertex* QuadVertexBufferBase = nullptr;
		u64 buffer_cursor = 0;

		std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
		u64 texture_slots_cursor{ 1 }; // 0 => white texture;

		v4 QuadVertexPositions{};

		Font* font;
		std::vector<FontAtlas> fontAtlas{};
	};

	static Renderer2DData s_Data;

	void Renderer2D::Init()
	{
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

		s_Data.font = new Font("assets/fonts/Poppins/Poppins-Medium.ttf");
		s_Data.fontAtlas.push_back(FontAtlas(s_Data.font->face, 24));
	}
	void Renderer2D::Shutdown()
	{
	}

	void Renderer2D::BeginScene(const Camera::OrthographicCamera& camera)
	{
		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
		//RenderCommand::enable_depth_testing(true);
		s_Data.QuadIndexCount = 0;
		s_Data.texture_slots_cursor = 1;
		s_Data.buffer_cursor = 0;
	}
	void Renderer2D::EndScene()
	{
		Flush();
	}

	void Renderer2D::Flush()
	{
		s_Data.QuadVertexBuffer->SetData((float*)s_Data.QuadVertexBufferBase, (u32)(s_Data.buffer_cursor * sizeof(QuadVertex)));

		s_Data.QuadVertexArray->Bind();

		for (size_t i = 0; i < s_Data.texture_slots_cursor; i++)
			s_Data.TextureSlots[i]->Bind((u32)i);
		RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);

		s_Data.QuadIndexCount = 0;
		s_Data.texture_slots_cursor = 1;
		s_Data.buffer_cursor = 0;
	}

	void Renderer2D::DrawQuad(const DrawQuadParameters& parameters)
	{
		glm::mat4 transform =
			glm::translate(glm::mat4(1.0f), parameters.Position)
			* glm::rotate(glm::mat4(1.0f), parameters.RotationInRadians, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), v3(parameters.Size, 1.0f));

		DrawQuad(transform, parameters);
	}
	void Renderer2D::DrawQuad(const glm::mat4& transform, const DrawQuadParameters& parameters)
	{
		float textureIndex = 0.0f;
		if (parameters.texture != nullptr)
		{
			for (size_t i = 1; i < s_Data.texture_slots_cursor; i++)
			{
				if (*s_Data.TextureSlots[i].get() == *parameters.texture.get())
				{
					textureIndex = (float)i;
					break;
				}
			}
			if (textureIndex == 0.0f)
			{
				textureIndex = (float)s_Data.texture_slots_cursor;
				s_Data.TextureSlots[s_Data.texture_slots_cursor] = parameters.texture;
				s_Data.texture_slots_cursor++;
			}
		}
		auto* QuadVertexBufferPtr = s_Data.QuadVertexBufferBase + s_Data.buffer_cursor;
		QuadVertexBufferPtr->Position = transform * (s_Data.QuadVertexPositions + v4{ parameters.trim[3], 1.0f - parameters.trim[2], 0.0f, 0.0f });
		QuadVertexBufferPtr->TintColor = parameters.TintColor;
		QuadVertexBufferPtr->TexCoord = { parameters.trim[3], 1.0f - parameters.trim[2] };
		QuadVertexBufferPtr->TextureIndex = textureIndex;
		QuadVertexBufferPtr->TilingFactor = 1.0f;
		s_Data.buffer_cursor++;

		QuadVertexBufferPtr = s_Data.QuadVertexBufferBase + s_Data.buffer_cursor;
		QuadVertexBufferPtr->Position = transform * (s_Data.QuadVertexPositions + v4{ parameters.trim[1], 1.0f - parameters.trim[2], 0.0f, 0.0f });;
		QuadVertexBufferPtr->TintColor = parameters.TintColor;
		QuadVertexBufferPtr->TexCoord = { parameters.trim[1], 1.0f - parameters.trim[2] };
		QuadVertexBufferPtr->TextureIndex = textureIndex;
		QuadVertexBufferPtr->TilingFactor = 1.0f;
		s_Data.buffer_cursor++;

		QuadVertexBufferPtr = s_Data.QuadVertexBufferBase + s_Data.buffer_cursor;
		QuadVertexBufferPtr->Position = transform * (s_Data.QuadVertexPositions + v4{ parameters.trim[1], 1.0f - parameters.trim[0], 0.0f, 0.0f });
		QuadVertexBufferPtr->TintColor = parameters.TintColor;
		QuadVertexBufferPtr->TexCoord = { parameters.trim[1], 1.0f - parameters.trim[0] };
		QuadVertexBufferPtr->TextureIndex = textureIndex;
		QuadVertexBufferPtr->TilingFactor = 1.0f;
		s_Data.buffer_cursor++;

		QuadVertexBufferPtr = s_Data.QuadVertexBufferBase + s_Data.buffer_cursor;
		QuadVertexBufferPtr->Position = transform * (s_Data.QuadVertexPositions + v4{ parameters.trim[3], 1.0f - parameters.trim[0], 0.0f, 0.0f });
		QuadVertexBufferPtr->TintColor = parameters.TintColor;
		QuadVertexBufferPtr->TexCoord = { parameters.trim[3], 1.0f - parameters.trim[0] };
		QuadVertexBufferPtr->TextureIndex = textureIndex;
		QuadVertexBufferPtr->TilingFactor = 1.0f;
		s_Data.buffer_cursor++;

		s_Data.QuadIndexCount += 6;
		Flush(); // WHYYYY this fixes visual glitches
	}

	void Renderer2D::DrawRoundedQuad(const DrawQuadParameters& parameters)
	{
		glm::mat4 transform =
			glm::translate(glm::mat4(1.0f), parameters.Position)
			* glm::rotate(glm::mat4(1.0f), parameters.RotationInRadians, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), v3(parameters.Size, 1.0f));

		DrawRoundedQuad(transform, parameters);
	}
	void Renderer2D::DrawRoundedQuad(const glm::mat4& transform, const DrawQuadParameters& parameters)
	{
		// radius / size;
		float textureIndex = 0.0f;
		if (parameters.texture != nullptr)
		{
			for (size_t i = 1; i < s_Data.texture_slots_cursor; i++)
			{
				if (*s_Data.TextureSlots[i].get() == *parameters.texture.get())
				{
					textureIndex = (float)i;
					break;
				}
			}
			if (textureIndex == 0.0f)
			{
				textureIndex = (float)s_Data.texture_slots_cursor;
				s_Data.TextureSlots[s_Data.texture_slots_cursor] = parameters.texture;
				s_Data.texture_slots_cursor++;
			}
		}
		float radiusX = parameters.radius / parameters.Size.x;
		float radiusY = parameters.radius / parameters.Size.y;

		auto* QuadVertexBufferPtr = s_Data.QuadVertexBufferBase + s_Data.buffer_cursor;
		QuadVertexBufferPtr->Position = transform * (s_Data.QuadVertexPositions + v4{ parameters.trim[3] + radiusX, 1.0f - parameters.trim[2], 0.0f, 0.0f });
		QuadVertexBufferPtr->TintColor = parameters.TintColor;
		QuadVertexBufferPtr->TexCoord = { parameters.trim[3] + radiusX, 1.0f - parameters.trim[2] };
		QuadVertexBufferPtr->TextureIndex = textureIndex;
		QuadVertexBufferPtr->TilingFactor = 1.0f;
		s_Data.buffer_cursor++;

		QuadVertexBufferPtr = s_Data.QuadVertexBufferBase + s_Data.buffer_cursor;
		QuadVertexBufferPtr->Position = transform * (s_Data.QuadVertexPositions + v4{ parameters.trim[1] - radiusX, 1.0f - parameters.trim[2], 0.0f, 0.0f });;
		QuadVertexBufferPtr->TintColor = parameters.TintColor;
		QuadVertexBufferPtr->TexCoord = { parameters.trim[1] - radiusX, 1.0f - parameters.trim[2] };
		QuadVertexBufferPtr->TextureIndex = textureIndex;
		QuadVertexBufferPtr->TilingFactor = 1.0f;
		s_Data.buffer_cursor++;

		QuadVertexBufferPtr = s_Data.QuadVertexBufferBase + s_Data.buffer_cursor;
		QuadVertexBufferPtr->Position = transform * (s_Data.QuadVertexPositions + v4{ parameters.trim[1] - radiusX, 1.0f - parameters.trim[0], 0.0f, 0.0f });
		QuadVertexBufferPtr->TintColor = parameters.TintColor;
		QuadVertexBufferPtr->TexCoord = { parameters.trim[1] - radiusX, 1.0f - parameters.trim[0] };
		QuadVertexBufferPtr->TextureIndex = textureIndex;
		QuadVertexBufferPtr->TilingFactor = 1.0f;
		s_Data.buffer_cursor++;

		QuadVertexBufferPtr = s_Data.QuadVertexBufferBase + s_Data.buffer_cursor;
		QuadVertexBufferPtr->Position = transform * (s_Data.QuadVertexPositions + v4{ parameters.trim[3] + radiusX, 1.0f - parameters.trim[0], 0.0f, 0.0f });
		QuadVertexBufferPtr->TintColor = parameters.TintColor;
		QuadVertexBufferPtr->TexCoord = { parameters.trim[3] + radiusX, 1.0f - parameters.trim[0] };
		QuadVertexBufferPtr->TextureIndex = textureIndex;
		QuadVertexBufferPtr->TilingFactor = 1.0f;
		s_Data.buffer_cursor++;

		QuadVertexBufferPtr = s_Data.QuadVertexBufferBase + s_Data.buffer_cursor;
		QuadVertexBufferPtr->Position = transform * (s_Data.QuadVertexPositions + v4{ parameters.trim[3], 1.0f - parameters.trim[2] + radiusY, 0.0f, 0.0f });
		QuadVertexBufferPtr->TintColor = parameters.TintColor;
		QuadVertexBufferPtr->TexCoord = { parameters.trim[3], 1.0f - parameters.trim[2] + radiusY };
		QuadVertexBufferPtr->TextureIndex = textureIndex;
		QuadVertexBufferPtr->TilingFactor = 1.0f;
		s_Data.buffer_cursor++;

		QuadVertexBufferPtr = s_Data.QuadVertexBufferBase + s_Data.buffer_cursor;
		QuadVertexBufferPtr->Position = transform * (s_Data.QuadVertexPositions + v4{ parameters.trim[1], 1.0f - parameters.trim[2] + radiusY, 0.0f, 0.0f });
		QuadVertexBufferPtr->TintColor = parameters.TintColor;
		QuadVertexBufferPtr->TexCoord = { parameters.trim[1], 1.0f - parameters.trim[2] + radiusY };
		QuadVertexBufferPtr->TextureIndex = textureIndex;
		QuadVertexBufferPtr->TilingFactor = 1.0f;
		s_Data.buffer_cursor++;

		QuadVertexBufferPtr = s_Data.QuadVertexBufferBase + s_Data.buffer_cursor;
		QuadVertexBufferPtr->Position = transform * (s_Data.QuadVertexPositions + v4{ parameters.trim[1], 1.0f - parameters.trim[0] - radiusY, 0.0f, 0.0f });
		QuadVertexBufferPtr->TintColor = parameters.TintColor;
		QuadVertexBufferPtr->TexCoord = { parameters.trim[1], 1.0f - parameters.trim[0] - radiusY };
		QuadVertexBufferPtr->TextureIndex = textureIndex;
		QuadVertexBufferPtr->TilingFactor = 1.0f;
		s_Data.buffer_cursor++;

		QuadVertexBufferPtr = s_Data.QuadVertexBufferBase + s_Data.buffer_cursor;
		QuadVertexBufferPtr->Position = transform * (s_Data.QuadVertexPositions + v4{ parameters.trim[3], 1.0f - parameters.trim[0] - radiusY, 0.0f, 0.0f });
		QuadVertexBufferPtr->TintColor = parameters.TintColor;
		QuadVertexBufferPtr->TexCoord = { parameters.trim[3], 1.0f - parameters.trim[0] - radiusY };
		QuadVertexBufferPtr->TextureIndex = textureIndex;
		QuadVertexBufferPtr->TilingFactor = 1.0f;
		s_Data.buffer_cursor++;

		s_Data.QuadIndexCount += 6 * 2;

		float angleStep = glm::radians(45.0f / parameters.radiusQuality);

		v2 op1{ -parameters.radius, +0.0f };
		v2 op2{ +0.0f, -parameters.radius };
		v2 op3{ +parameters.radius, +0.0f };
		v2 op4{ +0.0f, +parameters.radius };

		v2 tp1{ parameters.trim[3] + radiusX, 1.0f - parameters.trim[2] + radiusY };
		v2 tp2{ parameters.trim[1] - radiusX, 1.0f - parameters.trim[2] + radiusY };
		v2 tp3{ parameters.trim[1] - radiusX, 1.0f - parameters.trim[0] - radiusY };
		v2 tp4{ parameters.trim[3] + radiusX, 1.0f - parameters.trim[0] - radiusY };

		v3 pp1 = (v3)(transform * (s_Data.QuadVertexPositions + v4(tp1, 0.0f, 0.0f)));
		v3 pp2 = (v3)(transform * (s_Data.QuadVertexPositions + v4(tp2, 0.0f, 0.0f)));
		v3 pp3 = (v3)(transform * (s_Data.QuadVertexPositions + v4(tp3, 0.0f, 0.0f)));
		v3 pp4 = (v3)(transform * (s_Data.QuadVertexPositions + v4(tp4, 0.0f, 0.0f)));

		v2 offset(0.0f);
		for (size_t i = 0; i < parameters.radiusQuality; i++)
		{
			offset = glm::rotate(op1, angleStep * (0.0f + i * 2.0f));
			QuadVertexBufferPtr = s_Data.QuadVertexBufferBase + s_Data.buffer_cursor;
			QuadVertexBufferPtr->Position = pp1 + v3(offset, 0.0f);
			QuadVertexBufferPtr->TintColor = parameters.TintColor;
			QuadVertexBufferPtr->TexCoord = tp1 + offset / parameters.Size;
			QuadVertexBufferPtr->TextureIndex = textureIndex;
			QuadVertexBufferPtr->TilingFactor = 1.0f;
			s_Data.buffer_cursor++;

			offset = glm::rotate(op1, angleStep * (1.0f + i * 2.0f));
			QuadVertexBufferPtr = s_Data.QuadVertexBufferBase + s_Data.buffer_cursor;
			QuadVertexBufferPtr->Position = pp1 + v3(offset, 0.0f);
			QuadVertexBufferPtr->TintColor = parameters.TintColor;
			QuadVertexBufferPtr->TexCoord = tp1 + offset / parameters.Size;
			QuadVertexBufferPtr->TextureIndex = textureIndex;
			QuadVertexBufferPtr->TilingFactor = 1.0f;
			s_Data.buffer_cursor++;

			offset = glm::rotate(op1, angleStep * (2.0f + i * 2.0f));
			QuadVertexBufferPtr = s_Data.QuadVertexBufferBase + s_Data.buffer_cursor;
			QuadVertexBufferPtr->Position = pp1 + v3(offset, 0.0f);
			QuadVertexBufferPtr->TintColor = parameters.TintColor;
			QuadVertexBufferPtr->TexCoord = tp1 + offset / parameters.Size;
			QuadVertexBufferPtr->TextureIndex = textureIndex;
			QuadVertexBufferPtr->TilingFactor = 1.0f;
			s_Data.buffer_cursor++;

			QuadVertexBufferPtr = s_Data.QuadVertexBufferBase + s_Data.buffer_cursor;
			QuadVertexBufferPtr->Position = pp1;
			QuadVertexBufferPtr->TintColor = parameters.TintColor;
			QuadVertexBufferPtr->TexCoord = tp1;
			QuadVertexBufferPtr->TextureIndex = textureIndex;
			QuadVertexBufferPtr->TilingFactor = 1.0f;
			s_Data.buffer_cursor++;

			offset = glm::rotate(op2, angleStep * (0.0f + i * 2.0f));
			QuadVertexBufferPtr = s_Data.QuadVertexBufferBase + s_Data.buffer_cursor;
			QuadVertexBufferPtr->Position = pp2 + v3(offset, 0.0f);
			QuadVertexBufferPtr->TintColor = parameters.TintColor;
			QuadVertexBufferPtr->TexCoord = tp2 + offset / parameters.Size;
			QuadVertexBufferPtr->TextureIndex = textureIndex;
			QuadVertexBufferPtr->TilingFactor = 1.0f;
			s_Data.buffer_cursor++;

			offset = glm::rotate(op2, angleStep * (1.0f + i * 2.0f));
			QuadVertexBufferPtr = s_Data.QuadVertexBufferBase + s_Data.buffer_cursor;
			QuadVertexBufferPtr->Position = pp2 + v3(offset, 0.0f);
			QuadVertexBufferPtr->TintColor = parameters.TintColor;
			QuadVertexBufferPtr->TexCoord = tp2 + offset / parameters.Size;
			QuadVertexBufferPtr->TextureIndex = textureIndex;
			QuadVertexBufferPtr->TilingFactor = 1.0f;
			s_Data.buffer_cursor++;

			offset = glm::rotate(op2, angleStep * (2.0f + i * 2.0f));
			QuadVertexBufferPtr = s_Data.QuadVertexBufferBase + s_Data.buffer_cursor;
			QuadVertexBufferPtr->Position = pp2 + v3(offset, 0.0f);
			QuadVertexBufferPtr->TintColor = parameters.TintColor;
			QuadVertexBufferPtr->TexCoord = tp2 + offset / parameters.Size;
			QuadVertexBufferPtr->TextureIndex = textureIndex;
			QuadVertexBufferPtr->TilingFactor = 1.0f;
			s_Data.buffer_cursor++;

			QuadVertexBufferPtr = s_Data.QuadVertexBufferBase + s_Data.buffer_cursor;
			QuadVertexBufferPtr->Position = pp2;
			QuadVertexBufferPtr->TintColor = parameters.TintColor;
			QuadVertexBufferPtr->TexCoord = tp2;
			QuadVertexBufferPtr->TextureIndex = textureIndex;
			QuadVertexBufferPtr->TilingFactor = 1.0f;
			s_Data.buffer_cursor++;

			offset = glm::rotate(op3, angleStep * (0.0f + i * 2.0f));
			QuadVertexBufferPtr = s_Data.QuadVertexBufferBase + s_Data.buffer_cursor;
			QuadVertexBufferPtr->Position = pp3 + v3(offset, 0.0f);
			QuadVertexBufferPtr->TintColor = parameters.TintColor;
			QuadVertexBufferPtr->TexCoord = tp3 + offset / parameters.Size;
			QuadVertexBufferPtr->TextureIndex = textureIndex;
			QuadVertexBufferPtr->TilingFactor = 1.0f;
			s_Data.buffer_cursor++;

			offset = glm::rotate(op3, angleStep * (1.0f + i * 2.0f));
			QuadVertexBufferPtr = s_Data.QuadVertexBufferBase + s_Data.buffer_cursor;
			QuadVertexBufferPtr->Position = pp3 + v3(offset, 0.0f);
			QuadVertexBufferPtr->TintColor = parameters.TintColor;
			QuadVertexBufferPtr->TexCoord = tp3 + offset / parameters.Size;
			QuadVertexBufferPtr->TextureIndex = textureIndex;
			QuadVertexBufferPtr->TilingFactor = 1.0f;
			s_Data.buffer_cursor++;

			offset = glm::rotate(op3, angleStep * (2.0f + i * 2.0f));
			QuadVertexBufferPtr = s_Data.QuadVertexBufferBase + s_Data.buffer_cursor;
			QuadVertexBufferPtr->Position = pp3 + v3(offset, 0.0f);
			QuadVertexBufferPtr->TintColor = parameters.TintColor;
			QuadVertexBufferPtr->TexCoord = tp3 + offset / parameters.Size;
			QuadVertexBufferPtr->TextureIndex = textureIndex;
			QuadVertexBufferPtr->TilingFactor = 1.0f;
			s_Data.buffer_cursor++;

			QuadVertexBufferPtr = s_Data.QuadVertexBufferBase + s_Data.buffer_cursor;
			QuadVertexBufferPtr->Position = pp3;
			QuadVertexBufferPtr->TintColor = parameters.TintColor;
			QuadVertexBufferPtr->TexCoord = tp3;
			QuadVertexBufferPtr->TextureIndex = textureIndex;
			QuadVertexBufferPtr->TilingFactor = 1.0f;
			s_Data.buffer_cursor++;

			offset = glm::rotate(op4, angleStep * (0.0f + i * 2.0f));
			QuadVertexBufferPtr = s_Data.QuadVertexBufferBase + s_Data.buffer_cursor;
			QuadVertexBufferPtr->Position = pp4 + v3(offset, 0.0f);
			QuadVertexBufferPtr->TintColor = parameters.TintColor;
			QuadVertexBufferPtr->TexCoord = tp4 + offset / parameters.Size;
			QuadVertexBufferPtr->TextureIndex = textureIndex;
			QuadVertexBufferPtr->TilingFactor = 1.0f;
			s_Data.buffer_cursor++;

			offset = glm::rotate(op4, angleStep * (1.0f + i * 2.0f));
			QuadVertexBufferPtr = s_Data.QuadVertexBufferBase + s_Data.buffer_cursor;
			QuadVertexBufferPtr->Position = pp4 + v3(offset, 0.0f);
			QuadVertexBufferPtr->TintColor = parameters.TintColor;
			QuadVertexBufferPtr->TexCoord = tp4 + offset / parameters.Size;
			QuadVertexBufferPtr->TextureIndex = textureIndex;
			QuadVertexBufferPtr->TilingFactor = 1.0f;
			s_Data.buffer_cursor++;

			offset = glm::rotate(op4, angleStep * (2.0f + i * 2.0f));
			QuadVertexBufferPtr = s_Data.QuadVertexBufferBase + s_Data.buffer_cursor;
			QuadVertexBufferPtr->Position = pp4 + v3(offset, 0.0f);
			QuadVertexBufferPtr->TintColor = parameters.TintColor;
			QuadVertexBufferPtr->TexCoord = tp4 + offset / parameters.Size;
			QuadVertexBufferPtr->TextureIndex = textureIndex;
			QuadVertexBufferPtr->TilingFactor = 1.0f;
			s_Data.buffer_cursor++;

			QuadVertexBufferPtr = s_Data.QuadVertexBufferBase + s_Data.buffer_cursor;
			QuadVertexBufferPtr->Position = pp4;
			QuadVertexBufferPtr->TintColor = parameters.TintColor;
			QuadVertexBufferPtr->TexCoord = tp4;
			QuadVertexBufferPtr->TextureIndex = textureIndex;
			QuadVertexBufferPtr->TilingFactor = 1.0f;
			s_Data.buffer_cursor++;

			s_Data.QuadIndexCount += 6 * 4;
		}

		Flush(); // WHYYYY this fixes visual glitches
	}
	void Renderer2D::DrawText(const std::string& text, const v3& position, const v4& color, float zoomLevel)
	{
		int flags = FontFlags::LeftAligned | FontFlags::WordWrap;

		float _sx = zoomLevel * (2.0f / 1600.0f);
		float _sy = zoomLevel * (2.0f / 900.0f);


		float x = position.x;
		float y = position.y + (s_Data.font->face->size->metrics.height >> 6);

		FT_GlyphSlot slot = s_Data.font->face->glyph;

		// Calculate alignment (if applicable)
		float textWidth = 0;

		Char* chars = s_Data.fontAtlas[0].chars;
		for (const char* p = text.c_str(); *p; ++p) {
			textWidth += chars[*p].advanceX;
		}

		// Normalize window coordinates
		x = -1 + x * _sx;
		y = 1 - y * _sy;


		int atlasWidth = s_Data.fontAtlas[0].width;
		int atlasHeight = s_Data.fontAtlas[0].height;

		float textureIndex = 0.0f;

		for (size_t i = 1; i < s_Data.texture_slots_cursor; i++)
		{
			if (*s_Data.TextureSlots[i].get() == *s_Data.fontAtlas[0].texture.get())
			{
				textureIndex = (float)i;
				break;
			}
		}
		if (textureIndex == 0.0f)
		{
			textureIndex = (float)s_Data.texture_slots_cursor;
			s_Data.TextureSlots[s_Data.texture_slots_cursor] = s_Data.fontAtlas[0].texture;
			s_Data.texture_slots_cursor++;
		}

		for (const char* p = text.c_str(); *p; ++p) {
			float x2 = x + chars[*p].bitmapLeft * _sx; // scaled x coord
			float y2 = -y - chars[*p].bitmapTop * _sy; // scaled y coord
			float w = chars[*p].bitmapWidth * _sx;     // scaled width of character
			float h = chars[*p].bitmapHeight * _sy;    // scaled height of character

			// Calculate kerning value
			FT_Vector kerning;
			FT_Get_Kerning(s_Data.font->face,              // font face handle
				*p,                 // left glyph
				*(p + 1),           // right glyph
				FT_KERNING_DEFAULT, // kerning mode
				&kerning);          // variable to store kerning value

			// Advance cursor to start of next character
			x += (chars[*p].advanceX + (kerning.x >> 6)) * _sx;
			y += chars[*p].advanceY * _sy;

			// Skip glyphs with no pixels (e.g. spaces)
			if (!w || !h)
				continue;


			auto* QuadVertexBufferPtr = s_Data.QuadVertexBufferBase + s_Data.buffer_cursor;
			QuadVertexBufferPtr->Position = { x2 + w, -y2, 1.0f };
			QuadVertexBufferPtr->TintColor = color;
			QuadVertexBufferPtr->TexCoord = { chars[*p].xOffset + chars[*p].bitmapWidth / atlasWidth, 0.0f };
			QuadVertexBufferPtr->TextureIndex = textureIndex;
			QuadVertexBufferPtr->TilingFactor = 1.0f;
			s_Data.buffer_cursor++;

			QuadVertexBufferPtr = s_Data.QuadVertexBufferBase + s_Data.buffer_cursor;
			QuadVertexBufferPtr->Position = { x2,-y2, 1.0f };
			QuadVertexBufferPtr->TintColor = color;
			QuadVertexBufferPtr->TexCoord = { chars[*p].xOffset, 0.0f };
			QuadVertexBufferPtr->TextureIndex = textureIndex;
			QuadVertexBufferPtr->TilingFactor = 1.0f;
			s_Data.buffer_cursor++;

			QuadVertexBufferPtr = s_Data.QuadVertexBufferBase + s_Data.buffer_cursor;
			QuadVertexBufferPtr->Position = { x2, -y2 - h, 1.0f };
			QuadVertexBufferPtr->TintColor = color;
			QuadVertexBufferPtr->TexCoord = { chars[*p].xOffset, chars[*p].bitmapHeight / atlasHeight };
			QuadVertexBufferPtr->TextureIndex = textureIndex;
			QuadVertexBufferPtr->TilingFactor = 1.0f;
			s_Data.buffer_cursor++;

			QuadVertexBufferPtr = s_Data.QuadVertexBufferBase + s_Data.buffer_cursor;
			QuadVertexBufferPtr->Position = { x2 + w, -y2 - h, 1.0f };
			QuadVertexBufferPtr->TintColor = color;
			QuadVertexBufferPtr->TexCoord = { chars[*p].xOffset + chars[*p].bitmapWidth / atlasWidth, chars[*p].bitmapHeight / atlasHeight };
			QuadVertexBufferPtr->TextureIndex = textureIndex;
			QuadVertexBufferPtr->TilingFactor = 1.0f;
			s_Data.buffer_cursor++;

			s_Data.QuadIndexCount += 6;
		}

		Flush(); // WHYYYY this fixes visual glitches
	}
}