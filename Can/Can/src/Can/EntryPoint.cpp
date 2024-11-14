#include "canpch.h"
#if CAN_OPENGL
#include "EntryPoint.h"
#include "Core.h"
#include "Application.h"

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
	std::srand((unsigned int)std::time(nullptr));
	main_application = Can::CreateApplication(Can::WindowProps{ title, w, h });

	main_application->Run();

	delete main_application;
}
#endif // CAN_OPENGL

int __cdecl printf2(const char* format, ...)
{
	char str[1024];

	va_list argptr;
	va_start(argptr, format);
	int ret = vsnprintf(str, sizeof(str), format, argptr);
	va_end(argptr);

	OutputDebugStringA(str);

	return ret;
}