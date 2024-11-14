#include "canpch.h"
#if CAN_OPENGL
#include "EntryPoint.h"
#include "Core.h"
#include "Application.h"

extern Can::Application* Can::application_create(const Can::WindowProps& props); 

Can::Application* main_application = nullptr;

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	unsigned int w{ 1280 };
	unsigned int h{ 720 };
	std::string title{ "Application" };

	std::srand((unsigned int)std::time(nullptr));
	main_application = Can::application_create({ title, w, h });

	main_application->run();

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