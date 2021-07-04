#pragma once

#include <string>
#include "Can/Core.h"

namespace Can
{
	class Texture
	{
	public:
		virtual ~Texture() = default;
		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual void SetData(void* data, uint32_t size) = 0;
		virtual void SetSubData(void* data, uint32_t xoffset, uint32_t yoffset, uint32_t width, uint32_t height) = 0;

		virtual void Bind(uint32_t slot = 0) const = 0;
		virtual bool operator==(const Texture& other) const = 0;
	};

	class Texture2D :public Texture
	{
	public:
		static Ref<Texture2D> Create(uint32_t width, uint32_t height);
		static Ref<Texture2D> Create(const std::string& path);
		static Ref<Texture2D> Create(uint32_t id, uint32_t width, uint32_t height, const std::string& path);
	};
}