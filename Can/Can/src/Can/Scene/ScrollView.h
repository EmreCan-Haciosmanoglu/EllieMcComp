#pragma once
//#include "Can.h"
#undef min
#undef max
#include <entt/entt.hpp>

namespace Can
{
	struct ScrollBarConstructorParameters;
	class ScrollBar;
	struct ScrollViewConstructorParameters
	{
		entt::registry& Registry;
		entt::entity Parent = entt::null;
		glm::vec3 Position{ 0.0f, 0.0f, 0.0f };
		glm::vec2 Size{ 1.0f, 1.0f };
		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };

		uint8_t scrollPosition = 0;
		// 0 -> top
		// 1 -> right
		// 2 -> bottom
		// 3 -> left

		std::function<void()> OnClick;
		float borderRadius = 0.0f;
		bool border = false;
		glm::vec4 BorderColor{ 0.0f, 0.0f, 0.0f, 1.0f };
	};

	class ScrollView
	{
	public:
		ScrollView(ScrollViewConstructorParameters& parameters, ScrollBarConstructorParameters _ScrollBarConstructorParameters);
		~ScrollView();

		void Update();
	public:
		entt::registry& sceneRegistry;
		entt::entity entityID;

		ScrollBar* scrollbar = nullptr;
	};
}