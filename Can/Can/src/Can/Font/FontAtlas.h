#pragma once
#include "ft2build.h"
#include FT_FREETYPE_H
#include <glad/glad.h>
#include "Can/Renderer/Texture.h"
namespace Can
{
	struct Char
	{
        f32 advanceX;
        f32 advanceY;

        f32 bitmapWidth;
        f32 bitmapHeight;

        f32 bitmapLeft;
        f32 bitmapTop;

        f32 xOffset;
	};
	class FontAtlas
	{
    public:
        FontAtlas(FT_Face ft_face, u16 pixelSize);

        FT_Face face;
        FT_GlyphSlot slot;
        Ref<Texture2D> texture;

        Char chars[128];

        uint32_t width  = 0;
        uint32_t height = 0;
	};
}