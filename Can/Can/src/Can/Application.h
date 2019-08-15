#pragma once

#include "Core.h"
#include "Can/Events/Event.h"

namespace Can
{
	class CAN_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	};

	Application* CreateApplication();
}