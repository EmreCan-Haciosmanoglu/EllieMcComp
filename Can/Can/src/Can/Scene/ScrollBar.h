#pragma once
#include "Can.h"

namespace Can
{
	struct ScrollBarConstructorParameters
	{
		entt::registry& Registry;
		entt::entity Parent = entt::null;
		glm::vec3 Position{ 0.0f, 0.0f, 0.0f };
		glm::vec2 Size{ 1.0f, 1.0f };
		glm::vec4 BaseColor{ 1.0f, 1.0f, 1.0f, 1.0f };
		glm::vec4 BarColor{ 1.0f, 1.0f, 1.0f, 1.0f };
		bool vertical = false;
		bool fixedSize = true;
		float sizeRatio = 0.8f;
		float oversize = 1.0f; // TODO= Better name please
		float barPosition = 0.0f; // 0.0f <= t <= 1.0f
		std::function<void()> OnClick;
		std::function<void()> OnDrag;
		float barBorderRadius = 0.0f;
		float baseBorderRadius = 0.0f;
		bool baseBorder = false;
		bool barBorder = false;
		glm::vec4 baseBorderColor{ 0.0f, 0.0f, 0.0f, 1.0f };
		glm::vec4 barBorderColor{ 0.0f, 0.0f, 0.0f, 1.0f };
	};
	class ScrollBar
	{
	public:
		ScrollBar(const ScrollBarConstructorParameters& parameters);
		~ScrollBar();

		bool Update(const glm::vec2& mousePos);

	public:
		entt::registry& sceneRegistry;
		entt::entity entityID;

		bool vertical = false;
		bool fixedSize = true;
		float sizeRatio = 0.8f;
		float barPosition = 0.0f; // 0.0f <= t <= 1.0f
		std::function<void()> OnDragCallback;
	};
}