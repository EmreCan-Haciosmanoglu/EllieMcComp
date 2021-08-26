#pragma once

namespace Can
{
#define BUTTON_STATE_FLAGS_OVER     0x1
#define BUTTON_STATE_FLAGS_PRESSED  0x2
#define BUTTON_STATE_FLAGS_HOLD     0x4
#define BUTTON_STATE_FLAGS_RELEASED 0x8
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
	struct Slider_State
	{
		u16 flags = 0;
	};
}