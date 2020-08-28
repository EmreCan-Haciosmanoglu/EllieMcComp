#pragma once

#include "Core.h"

#ifdef CAN_PLATFORM_WINDOWS

extern Can::Application* Can::CreateApplication();

int main(int argc, char** argv)
{
	Can::Log::Init();
	CAN_CORE_WARN("Initialized Log!");
	
	CAN_PROFILE_BEGIN_SESSION("Startup", "CanProfile-Startup.json");
	std::srand((unsigned int)std::time(nullptr));
	auto app = Can::CreateApplication();
	CAN_PROFILE_END_SESSION();

	CAN_PROFILE_BEGIN_SESSION("Runtime", "CanProfile-Runtime.json");
	app->Run();
	CAN_PROFILE_END_SESSION();

	CAN_PROFILE_BEGIN_SESSION("Cleaning", "CanProfile-Cleaning.json");
	delete app;
	CAN_PROFILE_END_SESSION();
}

#endif // CAN_PLATFORM_WINDOWS