#include "canpch.h"
#include "Can.h"

#include "Can/EntryPoint.h"

#include "imgui.h"
#include <glm/gtc/type_ptr.hpp>

#include "GameLayer.h"


class _2048_Extended : public Can::Application
{
public:
	_2048_Extended()
	{
		PushLayer(new GameLayer());
	}

	~_2048_Extended()
	{

	}
};

Can::Application* Can::CreateApplication()
{
	return new _2048_Extended();
}