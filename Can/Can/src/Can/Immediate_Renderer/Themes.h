#pragma once

#include "Can/Core.h"

namespace Can
{
	struct Label_Theme
	{
		Font* font = nullptr;
		u16 font_size_in_pixel = 1;
		v4 color = v4(1.0f);
		u16 flags = 0;
	};

	struct Button_Theme
	{
		Label_Theme* label_theme = nullptr;
		v4 background_color = v4(1.0f);
		v4 background_color_over = v4(1.0f);
		v4 background_color_pressed = v4(1.0f);
	};

	struct Drop_Down_List_Theme
	{
		Button_Theme* button_theme = nullptr;
		Button_Theme* button_theme_selected = nullptr;

		v4 background_color = v4(1.0f);
	};

}