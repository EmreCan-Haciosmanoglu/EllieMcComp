#include "canpch.h"
#include "Platform.h"
#include "PlatformTypes.h"
#include "Can/Input/InputWin32.h"
#include "Can/Unordered_Array.h"

namespace Can::platform
{
#ifdef _WIN64
	namespace
	{
		struct window_info
		{
			HWND hwnd{ nullptr };
			RECT client_area{ 0, 0, 1920, 1080 };
			RECT fullscreen_area{};
			POINT top_left{ 0, 0 };
			DWORD style{ WS_VISIBLE };
			bool is_fullscreen{ false };
			bool is_closed{ false };
		};

		std::vector<window_info> windows{};

		void remove_from_windows(u32 id)
		{
			//assert(id < windows.capacity);
			//windows.values[id].valid = false;
		}

		window_info& get_from_id(window_id id)
		{
			assert(windows[id].hwnd);
			return windows[id];
		}

		window_info& get_from_handle(window_handle handle)
		{
			const window_id id{ (id::id_type)GetWindowLongPtr(handle, GWLP_USERDATA) };
			return get_from_id(id);
		}

		bool resized{ false };
		LRESULT CALLBACK internal_window_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
		{
			switch (msg)
			{
			case WM_NCCREATE:
			{
				DEBUG_OP(SetLastError(0));
				windows.push_back({ hwnd });
				SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)(windows.size() - 1));
				assert(GetLastError() == 0);
				break;
			}
			case WM_DESTROY:
			{
				get_from_handle(hwnd).is_closed = true;
				break;
			}
			case WM_SIZE:
			{
				resized = wparam != SIZE_MINIMIZED;
				break;
			}
			default:
				break;
			}

			input::process_input_message(hwnd, msg, wparam, lparam);

			if (resized && GetKeyState(VK_LBUTTON) >= 0)
			{
				window_info& info{ get_from_handle(hwnd) };
				assert(info.hwnd);
				GetClientRect(info.hwnd, info.is_fullscreen ? &info.fullscreen_area : &info.client_area);
				resized = false;
			}

			if (msg == WM_SYSCOMMAND && wparam == SC_KEYMENU)
			{
				return 0;
			}

			LONG_PTR long_ptr{ GetWindowLongPtr(hwnd, 0) };
			return long_ptr
				? ((window_proc)long_ptr)(hwnd, msg, wparam, lparam)
				: DefWindowProc(hwnd, msg, wparam, lparam);
		}

		void resize_window(const window_info& info, const RECT& area)
		{
			RECT window_rect{ area };
			AdjustWindowRect(&window_rect, info.style, FALSE);

			const s32 width{ window_rect.right - window_rect.left };
			const s32 height{ window_rect.bottom - window_rect.top };

			MoveWindow(info.hwnd, info.top_left.x, info.top_left.y, width, height, true);
		}

		void resize_window(window_id id, u32 width, u32 height)
		{
			window_info& info{ get_from_id(id) };
			RECT& area{ info.is_fullscreen ? info.fullscreen_area : info.client_area };
			area.bottom = area.top + height;
			area.right = area.left + width;

			resize_window(info, area);
		}

		void set_window_fullscreen(window_id id, bool is_fullscreen)
		{
			window_info& info{ get_from_id(id) };
			if (info.is_fullscreen != is_fullscreen)
			{
				info.is_fullscreen = is_fullscreen;
				if (is_fullscreen)
				{
					GetClientRect(info.hwnd, &info.client_area);
					RECT rect;
					GetWindowRect(info.hwnd, &rect);
					info.top_left.x = rect.left;
					info.top_left.y = rect.top;
					info.style = 0;
					SetWindowLongPtr(info.hwnd, GWL_STYLE, info.style);
					ShowWindow(info.hwnd, SW_MAXIMIZE);
				}
				else
				{
					info.style = WS_VISIBLE | WS_OVERLAPPEDWINDOW;
					SetWindowLongPtr(info.hwnd, GWL_STYLE, info.style);
					resize_window(info, info.client_area);
					ShowWindow(info.hwnd, SW_SHOWNORMAL);
				}
			}
		}

		bool is_window_fullscreen(window_id id)
		{
			return get_from_id(id).is_fullscreen;
		}

		void* get_window_handle(window_id id)
		{
			return get_from_id(id).hwnd;
		}

		void set_window_caption(window_id id, const wchar_t* caption)
		{
			window_info& info{ get_from_id(id) };
			SetWindowText(info.hwnd, caption);
		}

		//math::u32v4 get_window_size(window_id id)
		RECT get_window_size(window_id id)
		{
			window_info& info{ get_from_id(id) };
			RECT area{ info.is_fullscreen ? info.fullscreen_area : info.client_area };
			return area;
		}

		bool is_window_closed(window_id id)
		{
			return get_from_id(id).is_closed;
		}
	}

	window create_window(const window_init_info* const init_info /* = nullptr */)
	{
		window_proc callback{ init_info ? init_info->callback : nullptr };
		window_handle parent{ init_info ? init_info->parent : nullptr };

		WNDCLASSEX wc;
		ZeroMemory(&wc, sizeof(wc));
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = internal_window_proc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = callback ? sizeof(callback) : 0;
		wc.hInstance = 0;
		wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = CreateSolidBrush(RGB(26, 48, 76));
		wc.lpszMenuName = NULL;
		wc.lpszClassName = L"CanWindow";
		wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

		RegisterClassEx(&wc);

		window_info info{};
		info.client_area.right = (init_info && init_info->width) ? info.client_area.left + init_info->width : info.client_area.right;
		info.client_area.bottom = (init_info && init_info->height) ? info.client_area.top + init_info->height : info.client_area.bottom;
		info.style |= parent ? WS_CHILD : WS_OVERLAPPEDWINDOW;

		RECT rect{ info.client_area };

		AdjustWindowRect(&rect, info.style, FALSE);

		const wchar_t* caption{ (init_info && init_info->caption) ? init_info->caption : L"Can Game" };
		const s32 left{ init_info ? init_info->left : info.top_left.x };
		const s32 top{ init_info ? init_info->top : info.top_left.y };
		const s32 width{ rect.right - rect.left };
		const s32 height{ rect.bottom - rect.top };


		info.hwnd = CreateWindowEx(
			0,				   // extended style
			wc.lpszClassName,  // window class name
			caption,		   // instance title
			info.style,	       // window style
			left, top,		   // initial window position
			width, height,	   // initial window dimensionss
			parent,		       // handle to parent window
			NULL,			   // handle to menu
			NULL,			   // instance of this application
			NULL			   // extra creation parameters
		);

		if (info.hwnd)
		{
			DEBUG_OP(SetLastError(0));
			if (callback) SetWindowLongPtr(info.hwnd, 0, (LONG_PTR)callback);
			assert(GetLastError() == 0);

			ShowWindow(info.hwnd, SW_SHOWNORMAL);
			UpdateWindow(info.hwnd);

			const window_id id{ (id::id_type)GetWindowLongPtr(info.hwnd , GWLP_USERDATA) };
			windows[id] = info;
			return window{ id };
		}
		return {};
	}

	void remove_window(window_id id)
	{
		window_info& info{ get_from_id(id) };
		DestroyWindow(info.hwnd);
		remove_from_windows(id);
	}

#endif // _WIN64

	void window::set_fullscreen(bool is_fullscreen) const
	{
		assert(is_valid());
		set_window_fullscreen(_id, is_fullscreen);
	}

	bool window::is_fullscreen() const
	{
		assert(is_valid());
		return is_window_fullscreen(_id);
	}

	void* window::handle() const
	{
		assert(is_valid());
		return get_window_handle(_id);
	}

	void window::set_caption(const wchar_t* caption) const
	{
		assert(is_valid());
		set_window_caption(_id, caption);
	}

	//math::u32v4 window::size() const 
	RECT window::size() const
	{
		assert(is_valid());
		return get_window_size(_id);
	}

	void window::resize(u32 width, u32 height) const
	{
		assert(is_valid());
		resize_window(_id, width, height);
	}

	u32 window::width() const
	{
		RECT r{ size() };
		return r.right - r.left;
	}

	u32 window::height() const
	{
		RECT r{ size() };
		return r.bottom - r.top;
	}

	bool window::is_closed() const
	{
		assert(is_valid());
		return is_window_closed(_id);
	}
}