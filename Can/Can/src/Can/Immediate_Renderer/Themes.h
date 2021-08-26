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

		v4 background_color = v4(1.0f); // currently not being used
	};


#define SLIDER_THEME_FLAGS_THUMB_AT_TOP    0x1
#define SLIDER_THEME_FLAGS_THUMB_AT_BOTTOM 0x2
#define SLIDER_THEME_FLAGS_VALUE_SHOWN     0x4 
#define SLIDER_THEME_FLAGS_CLAMP_VALUE     0x8
	// CLAMP_MIN & CLAMP_MAX ???
	struct Slider_Theme
	{
		Button_Theme* thumb_theme = nullptr;
		Button_Theme* track_theme = nullptr;
		Label_Theme* label_theme = nullptr; // currently not being used

		u16 flags = 0;
		s32 y_offset_in_pixels = 0;
	};
}