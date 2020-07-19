#include <canpch.h>
#include "ChildrenComponent.h"

namespace Can
{
	ChildrenComponent::ChildrenComponent(std::vector<entt::entity> children)
		: Children(children)
	{
	}
}