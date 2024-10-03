#pragma once

#include "Can\CommonHeaders.h"
#include "Can\Platform\Window.h"

namespace Can::graphics
{
	class surface
	{};

	struct render_surface
	{
		platform::window* window{ nullptr };
		surface surface{};
	};

	enum class graphics_platform :u32
	{
		direct3d12 = 0,
	};

	bool initialize(graphics_platform platform);
	void shutdown();
	void render();
}