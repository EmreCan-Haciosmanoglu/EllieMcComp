#pragma once
#include "ft2build.h"
#include FT_FREETYPE_H
#include <string>

namespace Can
{
	class Font
	{
	public:
		Font(const std::string& path);

		void SetFont(const std::string& path);

		FT_Face GetFace() { return m_Face; }
	private:
		FT_Library m_Library;
		FT_Face m_Face;
	};
}