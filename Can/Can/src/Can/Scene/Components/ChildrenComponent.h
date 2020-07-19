#pragma once
#include "Can.h"

namespace Can
{
	struct ChildrenComponent
	{
		ChildrenComponent(const ChildrenComponent&) = default;
		ChildrenComponent(std::vector<entt::entity> children);

		operator std::vector<entt::entity>& () { return Children; }
		operator const std::vector<entt::entity>& () const { return Children; }

		entt::entity& operator[](size_t index) { return Children[index]; }
		const entt::entity& operator[](size_t index) const { return Children[index]; }

		std::vector<entt::entity>::iterator begin() { return Children.begin(); }
		std::vector<entt::entity>::const_iterator begin() const { return Children.begin(); }
		std::vector<entt::entity>::const_iterator cbegin() const { return Children.cbegin(); }
		
		std::vector<entt::entity>::iterator end() { return Children.end(); }
		std::vector<entt::entity>::const_iterator end() const { return Children.end(); }
		std::vector<entt::entity>::const_iterator cend() const { return Children.cend(); }

		std::vector<entt::entity> Children;
	};
}