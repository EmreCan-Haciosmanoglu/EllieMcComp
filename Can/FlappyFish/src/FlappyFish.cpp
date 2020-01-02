#include "canpch.h"
#include "Can.h"
#include "Can/EntryPoint.h"
#include "imgui.h"
#include <glm/gtc/type_ptr.hpp>
#include "GameLayer.h"

class FlappyBird : public Can::Application
{
public:
	FlappyBird()
	{
		ImGuiIO& io = ImGui::GetIO();
		io.Fonts->AddFontFromFileTTF("assets/fonts/Poppins-Light.otf", 32.0f);
		PushLayer(new GameLayer());
	}

	~FlappyBird()
	{

	}
};

Can::Application* Can::CreateApplication()
{
	return new FlappyBird();
}