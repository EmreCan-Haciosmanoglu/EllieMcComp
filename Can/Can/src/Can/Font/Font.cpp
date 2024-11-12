#include "canpch.h"
#include "Font.h"

namespace Can
{
	Font::Font(const std::string& path)
	{
		/*assert(!FT_Init_FreeType(&m_Library) && "Failed to initialize FreeType"); Im the most stupid person in the history*/
		FT_Error error = FT_Init_FreeType(&m_Library);
		assert(!error && "Failed to initialize FreeType");
		SetFont(path);
	}
	void Font::SetFont(const std::string& path)
	{
		// Create a new font
		FT_Error error = FT_New_Face(m_Library, path.c_str(), 0, &face);

		assert(error != FT_Err_Unknown_File_Format && "Font is not supported!");
		assert(!error &&  "Failed to open font");
		printf("%s font is imported\n", path.c_str());
	}
}