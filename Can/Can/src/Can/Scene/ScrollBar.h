#pragma once
#include "Can.h"

namespace Can
{
	struct ScrollBarConstructorParameters
	{
		entt::registry& Registry;
		entt::entity Parent = entt::null;
		const glm::vec3& Position = glm::vec3(0.0f);
		const glm::vec3& Size = glm::vec3(1.0f);
		const glm::vec4& BaseColor = glm::vec4(1.0f);
		const glm::vec4& BarColor = glm::vec4(1.0f);
		bool vertical = false;
		bool fixedSize = true;
		float sizeRatio = 0.8f;
		float oversize = 1.0f;// TODO= Better name please
		float barPosition = 0.0f; // 0.0f <= t <= 1.0f
		std::function<void()> OnClick; 
		std::function<void()> OnDrag;
	};
	class ScrollBar
	{
	public:
		ScrollBar(const ScrollBarConstructorParameters& parameters);

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