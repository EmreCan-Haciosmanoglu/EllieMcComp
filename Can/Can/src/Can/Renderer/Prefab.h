#pragma once
#include <string>
#include "Can/Renderer/Texture.h"
#include "Buffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include <glm/glm.hpp>
#undef min
#undef max
#include <limits>

namespace Can
{
	struct TexturedObjectVertex
	{
		v3 Position;
		v2 UV;
		v3 Normal;
		float TextureIndex;
	};

	#define MAX_TEXTURE_SLOTS 16
	class Prefab
	{
	public:
		Prefab() = default;
		Prefab(const std::string& objectPath, const std::string& shaderPath, const std::string& texturePath);
		Prefab(const std::string& objectPath, const std::string& shaderPath, const std::string& texturePath, float* vertices, size_t indexCount);
		Prefab(const std::string& objectPath, const std::string& shaderPath, const std::string& texturePath, float* vertices, size_t indexCount, size_t vertexCount, const BufferLayout& layout);
		Prefab(const std::string& objectPath, const std::string& shaderPath, const std::array<Ref<Texture2D>, MAX_TEXTURE_SLOTS>& textures, uint8_t textureCount, float* vertices, size_t indexCount, size_t vertexCount);
		~Prefab();

	public:
		std::string objectPath;
		std::string texturePath; // TODO: texturePaths
		std::string shaderPath;
		std::array<Ref<Texture2D>, MAX_TEXTURE_SLOTS> textures;
		size_t textureCount = 0;
		size_t indexCount;
		float* vertices;
		Ref<VertexBuffer> vertexBuffer;
		Ref<IndexBuffer> indexBuffer;
		Ref<VertexArray> vertexArray;


		Ref<Shader> shader;

		v3 boundingBoxL{ (std::numeric_limits<float>::max)() ,(std::numeric_limits<float>::max)(), (std::numeric_limits<float>::max)() };
		v3 boundingBoxM{ (std::numeric_limits<float>::min)() ,(std::numeric_limits<float>::min)(), (std::numeric_limits<float>::min)() };
	};
}