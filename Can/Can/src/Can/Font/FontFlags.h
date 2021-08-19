#pragma once
namespace Can
{
	enum FontFlags
	{
		LeftAligned      = 0x1,
		RightAligned     = 0x2,
		TopAligned       = 0x4,
		BottomAligned    = 0x8,
		WordWrap         = 0x10,
		Underlined       = 0x20,
		Bold             = 0x40,
		Italic           = 0x60,
		Indented         = 0x100,
		HorizontalLayout = 0x200
	};
}