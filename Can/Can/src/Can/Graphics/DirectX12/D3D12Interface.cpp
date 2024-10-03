#include "canpch.h"
#include "Can\CommonHeaders.h"
#include "D3D12Core.h"
#include "D3D12Interface.h"
#include "Can\Graphics\GraphicsPlatformInterface.h" 


namespace Can::graphics::d3d12
{
	void get_platform_interface(platform_interface& pi)
	{
		pi.initialize = core::initialize;
		pi.shutdown = core::shutdown;
		pi.render = core::render;
	}
}