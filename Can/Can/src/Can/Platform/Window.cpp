#include "canpch.h"
#include "Window.h"

namespace Can::platform
{
	namespace
	{

	}

	void window::set_fullscreen(bool is_fullscreen) const
	{
	}

	bool window::is_fullscreen() const
	{
		return false;
	}

	//void* handle() const;
	//void set_caption(const wchar_t* caption) const;
	//math::u32v4 size() const;
	//void resize(u32 width, u32 height) const;
	//u32 width() const;
	//u32 height() const;
	bool window::is_closed() const
	{
		return false;
	}

	window* create_window(const window_init_info* const info)
	{
		return new window();
	}

	void remove_window(const window_id& id)
	{

	}
}