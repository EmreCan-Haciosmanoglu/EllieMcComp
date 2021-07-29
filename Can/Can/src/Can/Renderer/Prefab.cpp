#include "canpch.h"
#include "Can.h"
#include "Prefab.h"

namespace Can
{
#define SCALE_DOWN 10.0f
	Prefab::Prefab(const std::string& objectPath, const std::string& shaderPath, const std::string& texturePath)
		: objectPath(objectPath)
		, texturePath(texturePath)
		, shaderPath(shaderPath)
	{
		CAN_PROFILE_FUNCTION();

		bool result = false;
		std::vector< glm::vec3 > vertexList;
		std::vector< glm::vec2 > uvList;
		std::vector< glm::vec3 > normalList;
		if (objectPath.find(".obj") != std::string::npos)
			result = AssetLoader::loadOBJ(objectPath.c_str(), vertexList, uvList, normalList);
		else
			std::cout << "Object file type is not supported!" << std::endl;

		if (result)
		{
			vertexArray = VertexArray::Create();
			indexCount = vertexList.size();

			TexturedObjectVertex* TOVertices = new TexturedObjectVertex[indexCount];
			for (int i = 0; i < indexCount; i++)
			{
				TOVertices[i].Position.x = vertexList[i].x / SCALE_DOWN;
				TOVertices[i].Position.y = vertexList[i].y / SCALE_DOWN;
				TOVertices[i].Position.z = (vertexList[i].z / SCALE_DOWN) + 0.01f;
				TOVertices[i].UV.x = uvList[i].x;
				TOVertices[i].UV.y = uvList[i].y;
				TOVertices[i].Normal.x = normalList[i].x;
				TOVertices[i].Normal.y = normalList[i].y;
				TOVertices[i].Normal.z = normalList[i].z;
				TOVertices[i].TextureIndex = 0.0f;

				boundingBoxL.x = std::min(boundingBoxL.x, TOVertices[i].Position.x);
				boundingBoxL.y = std::min(boundingBoxL.y, TOVertices[i].Position.y);
				boundingBoxL.z = std::min(boundingBoxL.z, TOVertices[i].Position.z);

				boundingBoxM.x = std::max(boundingBoxM.x, TOVertices[i].Position.x);
				boundingBoxM.y = std::max(boundingBoxM.y, TOVertices[i].Position.y);
				boundingBoxM.z = std::max(boundingBoxM.z, TOVertices[i].Position.z);
			}
			vertices = (float*)TOVertices;
			
			vertexBuffer = VertexBuffer::Create(vertices, (uint32_t)(indexCount * sizeof(TexturedObjectVertex)), true);
			vertexBuffer->SetLayout({
				{ ShaderDataType::Float3, "a_Position"},
				{ ShaderDataType::Float2, "a_UV"},
				{ ShaderDataType::Float3, "a_Normal"},
				{ ShaderDataType::Float, "a_TextureIndex"}
				});

			vertexArray->AddVertexBuffer(vertexBuffer);

			uint32_t* indices = new uint32_t[indexCount];

			for (int i = 0; i < indexCount; i++)
				indices[i] = i;

			indexBuffer = IndexBuffer::Create(indices, (uint32_t)indexCount);
			vertexArray->SetIndexBuffer(indexBuffer);

			textures[0] = Texture2D::Create(texturePath);
			textureCount = 1;

			shader = Shader::Create(shaderPath);

			shader->Bind();
			int32_t samplers[MAX_TEXTURE_SLOTS];
			for (int32_t i = 0; i < MAX_TEXTURE_SLOTS; i++)
				samplers[i] = i;
			shader->SetIntArray("u_Textures", samplers, MAX_TEXTURE_SLOTS);
			shader->SetInt("u_ShadowMap", 16);
			//shader->SetFloat3("u_LightPos", { 1.0f, 1.0f, -1.0f });

			delete[] indices;
		}
		else
			std::cout << "Object file couldn't process!" << std::endl;
	}

	Prefab::Prefab(const std::string& objectPath, const std::string& shaderPath, const std::string& texturePath, float* vertices, size_t indexCount)
		: objectPath(objectPath)
		, texturePath(texturePath)
		, shaderPath(shaderPath)
		, indexCount(indexCount)
		, vertices(vertices)
	{
		CAN_PROFILE_FUNCTION();

		vertexArray = VertexArray::Create();

		vertexBuffer = VertexBuffer::Create(vertices, (uint32_t)(indexCount * 9 * sizeof(float)), true);
		vertexBuffer->SetLayout({
		   { ShaderDataType::Float3, "a_Position"},
		   { ShaderDataType::Float2, "a_UV"},
		   { ShaderDataType::Float3, "a_Normal"},
		   { ShaderDataType::Float, "a_TextureIndex"}
			});

		vertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t* indices = new uint32_t[indexCount];

		for (int i = 0; i < indexCount; i++)
			indices[i] = i;

		indexBuffer = IndexBuffer::Create(indices, (uint32_t)indexCount);
		vertexArray->SetIndexBuffer(indexBuffer);

		textures[0] = Texture2D::Create(texturePath);
		textureCount = 1;

		shader = Shader::Create(shaderPath);

		shader->Bind();
		int32_t samplers[MAX_TEXTURE_SLOTS];
		for (int32_t i = 0; i < MAX_TEXTURE_SLOTS; i++)
			samplers[i] = i;
		shader->SetIntArray("u_Textures", samplers, MAX_TEXTURE_SLOTS);
		//shader->SetFloat3("u_LightPos", { 1.0f, 1.0f, -1.0f });

		delete[] indices;
	}

	Prefab::Prefab(const std::string& objectPath, const std::string& shaderPath, const std::array<Ref<Texture2D>, MAX_TEXTURE_SLOTS>& textures, uint8_t textureCount, float* vertices, size_t indexCount, size_t vertexCount)
		: objectPath(objectPath)
		, texturePath("")
		, shaderPath(shaderPath)
		, textures(textures)
		, textureCount(textureCount)
		, indexCount(indexCount)
		, vertices(vertices)
	{
		CAN_PROFILE_FUNCTION();

		vertexArray = VertexArray::Create();

		vertexBuffer = VertexBuffer::Create(vertices, (uint32_t)(vertexCount * sizeof(float)), true);
		vertexBuffer->SetLayout({
		   { ShaderDataType::Float3, "a_Position"},
		   { ShaderDataType::Float2, "a_UV"},
		   { ShaderDataType::Float3, "a_Normal"},
		   { ShaderDataType::Float, "a_TextureIndex"}
			});

		vertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t* indices = new uint32_t[indexCount];

		for (int i = 0; i < indexCount; i++)
			indices[i] = i;

		indexBuffer = IndexBuffer::Create(indices, (uint32_t)indexCount);
		vertexArray->SetIndexBuffer(indexBuffer);

		shader = Shader::Create(shaderPath);

		shader->Bind();
		int32_t samplers[MAX_TEXTURE_SLOTS];
		for (int32_t i = 0; i < MAX_TEXTURE_SLOTS; i++)
			samplers[i] = i;
		shader->SetIntArray("u_Textures", samplers, MAX_TEXTURE_SLOTS);
		//shader->SetFloat3("u_LightPos", { 1.0f, 1.0f, -1.0f });

		delete[] indices;
	}

	Prefab::Prefab(const std::string& objectPath, const std::string& shaderPath, const std::string& texturePath, float* vertices, size_t indexCount, size_t vertexCount, const BufferLayout& layout)
		: objectPath(objectPath)
		, texturePath(texturePath)
		, shaderPath(shaderPath)
		, indexCount(indexCount)
		, vertices(vertices)
	{
		CAN_PROFILE_FUNCTION();

		vertexArray = VertexArray::Create();

		vertexBuffer = VertexBuffer::Create(vertices, (uint32_t)(sizeof(float) * vertexCount), true);
		vertexBuffer->SetLayout(layout);

		vertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t* indices = new uint32_t[indexCount];

		for (int i = 0; i < indexCount; i++)
			indices[i] = i;

		indexBuffer = IndexBuffer::Create(indices, (uint32_t)indexCount);
		vertexArray->SetIndexBuffer(indexBuffer);

		if (texturePath.compare("") != 0)
		{
			textures[0] = Texture2D::Create(texturePath);
			textureCount = 1;
		}

		shader = Shader::Create(shaderPath);

		shader->Bind();
		int32_t samplers[MAX_TEXTURE_SLOTS];
		for (int32_t i = 0; i < MAX_TEXTURE_SLOTS; i++)
			samplers[i] = i;
		shader->SetIntArray("u_Textures", samplers, MAX_TEXTURE_SLOTS);
		//shader->SetFloat3("u_LightPos", { 1.0f, 1.0f, -1.0f });

		delete[] indices;
	}

	Prefab::~Prefab()
	{
		delete[] vertices;
	}
}