#pragma once
#include "Can.h"

namespace Can
{
	struct ChildrenComponent
	{
		ChildrenComponent() = default;
		ChildrenComponent(const ChildrenComponent&) = default;
		ChildrenComponent(std::vector<entt::entity> children);

		operator std::vector<entt::entity>& () { return Children; }
		operator const std::vector<entt::entity>& () const { return Children; }

		entt::entity& operator[](size_t index) { return Children[index]; }
		const entt::entity& operator[](size_t index) const { return Children[index]; }

		std::vector<entt::entity> Children;
	};
}