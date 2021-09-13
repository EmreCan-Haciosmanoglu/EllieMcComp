#include "canpch.h"
#include "FontAtlas.h"
#include "Can/Renderer/Texture.h"
#include <glad/glad.h>

namespace Can
{
	FontAtlas::FontAtlas(FT_Face ft_face, u16 pixelSize)
		: face(ft_face)
		, slot(face->glyph)
	{
		FT_Set_Pixel_Sizes(face, 0, pixelSize);

		int rowWidth = 0;
		int colHeight = 0;

		for (int i = 32; i < MAX_UNICODE_CHAR_COUNT; i++)
		{
			FT_Error error = FT_Load_Char(face, i, FT_LOAD_RENDER);
			if (error)
			{
				printf("Loading character %c failed", i);
				continue;
			}
			width += slot->bitmap.width + 2;
			height = (std::max)(height, (uint32_t)slot->bitmap.rows);
		}
		texture = Texture2D::Create(width, height);

		int texPos = 0;
		for (int i = 32; i < MAX_UNICODE_CHAR_COUNT; ++i) {
			if (FT_Load_Char(face, i, FT_LOAD_RENDER))
				continue;

			size_t size = slot->bitmap.width * slot->bitmap.rows;
			std::vector<GLubyte> Data(size * 4, 0);
			for (size_t i = 0; i < size; i++)
			{
				unsigned char c = slot->bitmap.buffer[i];
				size_t index = i * 4;
				Data[index + 0] = c;
				Data[index + 1] = c;
				Data[index + 2] = c;
				Data[index + 3] = c;
			}
			texture->SetSubData(Data.data(), texPos, 0, slot->bitmap.width, slot->bitmap.rows);


			chars[i].advanceX = slot->advance.x >> 6;
			chars[i].advanceY = slot->advance.y >> 6;

			chars[i].bitmapWidth = slot->bitmap.width;
			chars[i].bitmapHeight = slot->bitmap.rows;

			chars[i].bitmapLeft = slot->bitmap_left;
			chars[i].bitmapTop = slot->bitmap_top;

			chars[i].xOffset = (float)texPos / (float)width;
			texPos += slot->bitmap.width + 2;
		}
	}
}