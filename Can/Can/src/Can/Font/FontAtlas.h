#pragma once
#include "ft2build.h"
#include FT_FREETYPE_H
#include <glad/glad.h>
#include "Can/Renderer/Texture.h"
namespace Can
{
	struct Char
	{
        float advanceX;
        float advanceY;

        float bitmapWidth;
        float bitmapHeight;

        float bitmapLeft;
        float bitmapTop;

        float xOffset;
	};
	class FontAtlas
	{
    public:
        FontAtlas(FT_Face face, int pixelSize);

        FT_Face face;
        FT_GlyphSlot slot;
        Ref<Texture2D> texture;

        Char chars[128];

        uint32_t width  = 0;
        uint32_t height = 0;
	};
}