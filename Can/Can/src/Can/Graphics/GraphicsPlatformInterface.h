#pragma once

#include "Can\CommonHeaders.h"
#include "Renderer.h"

namespace Can::graphics
{
	struct platform_interface
	{
		bool(*initialize)(void);
		void(*shutdown)(void);
		void(*render)(void);
	};
}