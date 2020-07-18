#pragma once
#include "Can.h"

namespace Can
{
	struct EnabledComponent
	{
		EnabledComponent(bool enabled = false);
		EnabledComponent(const EnabledComponent&) = default;
		

		operator bool& () { return Enabled; }
		operator const bool& () const { return Enabled; }

		bool Enabled;
	};
}