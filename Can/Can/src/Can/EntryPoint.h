#pragma once

#ifdef CAN_PLATFORM_WINDOWS

extern Can::Application* Can::CreateApplication();

int main(int argc, char** argv)
{
	Can::Log::Init();
	std::srand((unsigned int)std::time(nullptr));
	auto app = Can::CreateApplication();
	app->Run();
	delete app;
}

#endif // CAN_PLATFORM_WINDOWS