#include "canpch.h"
#include "ParentComponent.h"

namespace Can
{
	ParentComponent::ParentComponent()
		: Parent(entt::null)
	{
	}
	ParentComponent::ParentComponent(entt::entity parent)
		: Parent(parent)
	{
	}
}