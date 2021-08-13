#include "canpch.h"
#include "EntryPoint.h"
#include "Core.h"
#include "Application.h"

#ifdef CAN_PLATFORM_WINDOWS

extern Can::Application* Can::CreateApplication(const Can::WindowProps& props); 

Can::Application* main_application = nullptr;

int main(int argc, char** argv)
{
	unsigned int w = 1280;
	unsigned int h = 720;
	std::string title = "Application";
	for (size_t i = 0; i < argc - 1; i++)
	{
		if (strcmp(argv[i], "-t") == 0 || strcmp(argv[i], "--title") == 0)
		{
			title = argv[i + 1];
			i++;
		}
		else if (strcmp(argv[i], "-w") == 0 || strcmp(argv[i], "--width") == 0)
		{
			w = std::stoi(argv[i + 1]);
			i++;
		}
		else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--height") == 0)
		{
			h = std::stoi(argv[i + 1]);
			i++;
		}
	}
	Can::Log::Init();
	CAN_CORE_WARN("Initialized Log!");

	CAN_PROFILE_BEGIN_SESSION("Startup", "CanProfile-Startup.json");
	std::srand((unsigned int)std::time(nullptr));
	main_application = Can::CreateApplication(Can::WindowProps{ title, w, h });
	CAN_PROFILE_END_SESSION();

	CAN_PROFILE_BEGIN_SESSION("Runtime", "CanProfile-Runtime.json");
	main_application->Run();
	CAN_PROFILE_END_SESSION();

	CAN_PROFILE_BEGIN_SESSION("Cleaning", "CanProfile-Cleaning.json");
	delete main_application;
	CAN_PROFILE_END_SESSION();
}

#endif // CAN_PLATFORM_WINDOWS