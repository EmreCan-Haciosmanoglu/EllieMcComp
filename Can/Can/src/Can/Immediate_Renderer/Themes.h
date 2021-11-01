#pragma once

#include "Can/Core.h"

namespace Can
{
	struct Label_Theme
	{
		Font* font = nullptr;
		u16 font_size_in_pixel = 1;
		v4 color = v4(1.0f);
		// v4 color_over = v4(1.0f);
		// v4 color_selected = v4(1.0f);
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


#define SLIDER_THEME_FLAGS_THUMB_AT_TOP_OR_RIGHT   0x1
#define SLIDER_THEME_FLAGS_THUMB_AT_BOTTOM_OR_LEFT 0x2
#define SLIDER_THEME_FLAGS_VALUE_SHOWN             0x4 
#define SLIDER_THEME_FLAGS_CLAMP_VALUE             0x8
#define SLIDER_THEME_FLAGS_THUMB_IS_INSIDE         0x10
#define SLIDER_THEME_FLAGS_IS_HORIZONTAL           0x20
	// CLAMP_MIN & CLAMP_MAX ???
	struct Slider_Theme
	{
		Button_Theme* thumb_theme = nullptr;
		Button_Theme* track_theme = nullptr;
		Label_Theme* label_theme = nullptr; // currently not being used

		u16 flags = 0;
		s32 x_or_y_offset_in_pixels = 0;
	};

#define SUB_REGION_THEME_FLAGS_VERTICALLY_SCROLLABLE   0x1
#define SUB_REGION_THEME_FLAGS_HORIZONTALLY_SCROLLABLE 0x2
	struct Sub_Region_Theme
	{
		Slider_Theme* vertical_slider_theme = nullptr;
		Slider_Theme* horizontal_slider_theme = nullptr;

		v4 background_color = v4(1.0f);

		u16 flags = 0;
	};

	struct Check_Box_Theme
	{
		v4 background_color = v4(1.0f);
		v4 background_color_over = v4(1.0f);
		v4 background_color_pressed = v4(1.0f);
		v4 background_color_checked = v4(1.0f);
		v4 background_color_unchecked = v4(1.0f);

		s32 padding = 5;
	};

	struct Text_Box_Theme
	{
		Label_Theme* label_theme = nullptr;

		v4 border_color = v4(1.0f); // currently not being used
		v4 background_color = v4(1.0f);
		v4 background_color_active = v4(1.0f);
		//v4 background_color_over = v4(1.0f);
	};
}