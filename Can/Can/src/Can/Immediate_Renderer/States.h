#pragma once

namespace Can
{
#define BUTTON_STATE_FLAGS_OVER         0x1
#define BUTTON_STATE_FLAGS_PRESSED      0x2
#define BUTTON_STATE_FLAGS_HOLD         0x4
#define BUTTON_STATE_FLAGS_RELEASED     0x8
#define BUTTON_STATE_FLAGS_FULLY_HIDDEN 0x10
	struct Button_State
	{
		u16 flags = 0;
	};

#define DROP_DOWN_LIST_STATE_FLAGS_OVER         0x1
#define DROP_DOWN_LIST_STATE_FLAGS_PRESSED      0x2
#define DROP_DOWN_LIST_STATE_FLAGS_HOLD         0x4
#define DROP_DOWN_LIST_STATE_FLAGS_RELEASED     0x8
#define DROP_DOWN_LIST_STATE_FLAGS_ITEM_CHANGED 0x10
#define DROP_DOWN_LIST_STATE_FLAGS_OPEN         0x20
#define DROP_DOWN_LIST_STATE_FLAGS_FULLY_HIDDEN 0x40
	struct Drop_Down_List_State
	{
		u16 flags = 0;
		u64 active_item = 0;
	};

#define SLIDER_STATE_FLAGS_OVER           0x1
#define SLIDER_STATE_FLAGS_THUMB_PRESSED  0x2
#define SLIDER_STATE_FLAGS_THUMB_HOLD     0x4
#define SLIDER_STATE_FLAGS_THUMB_RELEASED 0x8
#define SLIDER_STATE_FLAGS_VALUE_CHANGED  0x10
#define SLIDER_STATE_FLAGS_FULLY_HIDDEN   0x20
	struct Slider_State
	{
		u16 flags = 0;
	};

#define SUB_REGION_STATE_FLAGS_OVER         0x1
#define SUB_REGION_STATE_FLAGS_FULLY_HIDDEN 0x2
	struct Sub_Region_State
	{
		u16 flags = 0;
		f32 horizontal_scroll_percentage = 0.0f;
		f32 vertical_scroll_percentage = 0.0f;
	};

#define CHECK_BOX_STATE_FLAGS_OVER          0x1
#define CHECK_BOX_STATE_FLAGS_PRESSED       0x2
#define CHECK_BOX_STATE_FLAGS_HOLD          0x4
#define CHECK_BOX_STATE_FLAGS_RELEASED      0x8
#define CHECK_BOX_STATE_FLAGS_VALUE_CHANGED 0x10
	struct Check_Box_State
	{
		u16 flags = 0;
	};

#define TEXT_BOX_STATE_FLAGS_OVER     0x1
#define TEXT_BOX_STATE_FLAGS_PRESSED  0x2
#define TEXT_BOX_STATE_FLAGS_RELEASED 0x4
#define TEXT_BOX_STATE_FLAGS_ACTIVE   0x8
	struct Text_Box_State
	{
		u16 flags = 0;
		u16 cursor = 0;
		u16 char_count = 0;
		u16 capacity = 0;
		std::string text = "";
	};
}