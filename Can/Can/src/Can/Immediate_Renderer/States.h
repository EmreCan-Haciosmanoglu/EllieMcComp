#pragma once

namespace Can
{
#define BUTTON_STATE_FLAGS_OVER     0x1
#define BUTTON_STATE_FLAGS_PRESSED  0x2
#define BUTTON_STATE_FLAGS_HOLD     0x4
#define BUTTON_STATE_FLAGS_RELEASED 0x8

	struct Button_State
	{
		u8 flags = 0;
	};
}