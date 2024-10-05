#include "canpch.h"
#include "Platform.h"
#include "PlatformTypes.h"

namespace Can::platform
{
#ifdef _WIN64
	namespace
	{
		struct window_info
		{
			HWND hwnd{ nullptr };
			RECT clict_area{ 0, 0, 1920, 1080 };
			RECT fullscreen_area{};
			POINT top_left{ 0, 0 };
			DWORD is_fullscreen{ false };
			DWORD is_closed{ false };
		};

		std::vector<window_info> windows;
		std::vector<u32> available_slots;

		u32 add_to_windows(window_info info)
		{
			u32 id{ u32_invalid_id };
			if (available_slots.empty())
			{
				id = (u32)windows.size();
				windows.emplace_back(info);
			}
			else
			{
				id = available_slots.back();
				available_slots.pop_back();
				assert(id != u32_invalid_id);
				windows[id] = info;
			}
			return id;
		}
	}
#endif // _WIN64

}