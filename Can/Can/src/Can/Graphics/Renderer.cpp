#include "canpch.h"
#include "Renderer.h"
#include "GraphicsPlatformInterface.h"
#include "DirectX12/D3D12Interface.h"

namespace Can::graphics
{
	namespace
	{
		platform_interface gfx{};

		bool set_platform_interface(graphics_platform platform)
		{
			switch (platform)
			{
			case Can::graphics::graphics_platform::direct3d12:
			{
				d3d12::get_platform_interface(gfx);
				break;
			}
			default:
				return false;
				break;
			}
			return true; 
		}
	}

	bool initialize(graphics_platform platform)
	{
		return set_platform_interface(platform) && gfx.initialize();
	}

	void shutdown()
	{
		gfx.shutdown();
	}

	void render()
	{
		gfx.render();
	}
}