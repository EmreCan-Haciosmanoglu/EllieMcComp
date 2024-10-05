#pragma once
#include "Can\CommonHeaders.h"
#include "Window.h"

namespace Can::platform
{
	struct window_init_info;

	window create_window(const window_init_info* const init_info = nullptr);
	void remove_window(window_id id);
}