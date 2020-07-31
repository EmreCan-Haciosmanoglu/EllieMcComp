#include "canpch.h"
#include "Prefab.h"

namespace Can
{
	Prefab::Prefab(const std::string& objectPath, const std::string& shaderPath, const std::string& texturePath)
		: objectPath(objectPath)
		, texturePath(texturePath)
		, shaderPath(shaderPath)
	{
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
			vertexCount = indexCount * (3 + 2 + 3);

			vertices = new float[vertexCount];
			for (int i = 0; i < indexCount; i++)
			{
				int index = i * 8;
				vertices[index + 0] = vertexList[i].x;
				vertices[index + 1] = vertexList[i].y;
				vertices[index + 2] = vertexList[i].z;
				vertices[index + 3] = uvList[i].x;
				vertices[index + 4] = uvList[i].y;
				vertices[index + 5] = normalList[i].x;
				vertices[index + 6] = normalList[i].y;
				vertices[index + 7] = normalList[i].z;

				boundingBoxL.x = std::min(boundingBoxL.x, vertexList[i].x);
				boundingBoxL.y = std::min(boundingBoxL.y, vertexList[i].y);
				boundingBoxL.z = std::min(boundingBoxL.z, vertexList[i].z);

				boundingBoxM.x = std::max(boundingBoxM.x, vertexList[i].x);
				boundingBoxM.y = std::max(boundingBoxM.y, vertexList[i].y);
				boundingBoxM.z = std::max(boundingBoxM.z, vertexList[i].z);
			}

			vertexBuffer = VertexBuffer::Create(vertices, sizeof(float) * vertexCount, true);
			vertexBuffer->SetLayout({
			   { ShaderDataType::Float3, "a_Position"},
			   { ShaderDataType::Float2, "a_UV"},
			   { ShaderDataType::Float3, "a_Normal"}
				});

			vertexArray->AddVertexBuffer(vertexBuffer);

			uint32_t* indices = new uint32_t[indexCount];

			for (int i = 0; i < indexCount; i++)
				indices[i] = i;

			indexBuffer = IndexBuffer::Create(indices, indexCount);
			vertexArray->SetIndexBuffer(indexBuffer);

			texture = Texture2D::Create(texturePath);
			shader = Shader::Create(shaderPath);

			shader->Bind();
			shader->SetInt("u_Texture", 0);
			shader->SetFloat3("u_LightPos", { 1.0f, 1.0f, -1.0f });

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
		, vertexCount(indexCount * (3 + 2 + 3))
		, vertices(vertices)
	{
		vertexArray = VertexArray::Create();

		vertexBuffer = VertexBuffer::Create(vertices, sizeof(float) * vertexCount, true);
		vertexBuffer->SetLayout({
		   { ShaderDataType::Float3, "a_Position"},
		   { ShaderDataType::Float2, "a_UV"},
		   { ShaderDataType::Float3, "a_Normal"}
			});

		vertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t* indices = new uint32_t[indexCount];

		for (int i = 0; i < indexCount; i++)
			indices[i] = i;

		indexBuffer = IndexBuffer::Create(indices, indexCount);
		vertexArray->SetIndexBuffer(indexBuffer);

		texture = Texture2D::Create(texturePath);
		shader = Shader::Create(shaderPath);

		shader->Bind();
		shader->SetInt("u_Texture", 0);
		shader->SetFloat3("u_LightPos", { 1.0f, 1.0f, -1.0f });

		delete[] indices;
	}

	Prefab::Prefab(const std::string& objectPath, const std::string& shaderPath, const std::string& texturePath, float* vertices, size_t indexCount, size_t vertexCount, const BufferLayout& layout)
		: objectPath(objectPath)
		, texturePath(texturePath)
		, shaderPath(shaderPath)
		, indexCount(indexCount)
		, vertexCount(vertexCount)
		, vertices(vertices)
	{
		vertexArray = VertexArray::Create();

		vertexBuffer = VertexBuffer::Create(vertices, sizeof(float) * vertexCount, true);
		vertexBuffer->SetLayout(layout);

		vertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t* indices = new uint32_t[indexCount];

		for (int i = 0; i < indexCount; i++)
			indices[i] = i;

		indexBuffer = IndexBuffer::Create(indices, indexCount);
		vertexArray->SetIndexBuffer(indexBuffer);

		if(texturePath.compare("") != 0)
			texture = Texture2D::Create(texturePath);

		shader = Shader::Create(shaderPath);

		shader->Bind();
		shader->SetInt("u_Texture", 0);
		shader->SetFloat3("u_LightPos", { 1.0f, 1.0f, -1.0f });

		delete[] indices;
	}
	
	Prefab::~Prefab()
	{
		delete[] vertices;
	}
}