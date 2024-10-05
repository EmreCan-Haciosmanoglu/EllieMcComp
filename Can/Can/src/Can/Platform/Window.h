#pragma once
#include "Can\Core.h"
#include "Can\CommonHeaders.h"

namespace Can::platform
{

	struct window_init_info
	{
		LRESULT(*win_proc)(HWND, UINT, WPARAM, LPARAM);
		int* a;
		LPCWSTR lpcwstr;
		s32 x, y, w, h;
	};

	DEFINE_TYPED_ID(window_id);

	class window
	{
	public:
		constexpr explicit window(window_id id) : _id{ id } {}
		constexpr window() = default;
		constexpr window_id get_id() const { return _id; }
		//constexpr bool is_valid() const { return id::is_valid(_id); }
		constexpr bool is_valid() const { return true; }

		void set_fullscreen(bool is_fullscreen) const;
		bool is_fullscreen() const;
		void* handle() const;
		//void set_caption(const wchar_t* caption) const;
		//math::u32v4 size() const;
		//void resize(u32 width, u32 height) const;
		u32 width() const;
		u32 height() const;
		bool is_closed() const;

	private:
		window_id _id{};// id::invalid_id
	};

	window create_window(const window_init_info* const info);
	void remove_window(const window_id& id);
}