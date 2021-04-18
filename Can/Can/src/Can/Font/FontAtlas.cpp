#include "canpch.h"
#include "FontAtlas.h"
#include "Can/Renderer/Texture.h"
#include <glad/glad.h>

namespace Can
{
	FontAtlas::FontAtlas(FT_Face face, int pixelSize)
		: face(face)
		, slot(face->glyph)
	{
		FT_Set_Pixel_Sizes(face, 0, pixelSize);

		int rowWidth = 0;
		int colHeight = 0;

		for (int i = 32; i < 128; i++)
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
		texture = Texture2D::Create(width, height, (unsigned int)GL_RED);

		int texPos = 0;
		std::vector<GLubyte> emptyData(1 * slot->bitmap.rows, 0);
		for (int i = 32; i < 128; ++i) {
			if (FT_Load_Char(face, i, FT_LOAD_RENDER))
				continue;
			texture->SetSubData(emptyData.data(), texPos, 0, 1, slot->bitmap.rows);
			//texPos += 1;
			texture->SetSubData(slot->bitmap.buffer, texPos, 0, slot->bitmap.width, slot->bitmap.rows);
			//texPos += slot->bitmap.width;
			texture->SetSubData(emptyData.data(), texPos, 0, 1, slot->bitmap.rows);
			//texPos += 1;

			// Store glyph info in our char array for this pixel size
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