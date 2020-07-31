#pragma once
#include "Can.h"

namespace Can
{
	class Prefab
	{
	public:
		Prefab() = default;
		Prefab(const std::string& objectPath, const std::string& shaderPath, const std::string& texturePath);
		Prefab(const std::string& objectPath, const std::string& shaderPath, const std::string& texturePath, float* vertices, size_t indexCount);
		Prefab(const std::string& objectPath, const std::string& shaderPath, const std::string& texturePath, float* vertices, size_t indexCount, size_t vertexCount,const BufferLayout& layout);
		~Prefab();

	public:
		std::string objectPath;
		size_t indexCount;
		size_t vertexCount;
		float* vertices;
		Ref<VertexBuffer> vertexBuffer;
		Ref<IndexBuffer> indexBuffer;
		Ref<VertexArray> vertexArray;

		std::string texturePath;
		Ref<Texture2D> texture;

		std::string shaderPath;
		Ref<Shader> shader;

		glm::vec3 boundingBoxL{ std::numeric_limits<float>::max() ,std::numeric_limits<float>::max() ,std::numeric_limits<float>::max() };
		glm::vec3 boundingBoxM{ std::numeric_limits<float>::min() ,std::numeric_limits<float>::min() ,std::numeric_limits<float>::min() };
	};
}