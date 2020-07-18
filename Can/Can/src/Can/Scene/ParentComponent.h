#pragma once
#include "Can.h"

namespace Can
{
	struct ParentComponent
	{
		ParentComponent();
		ParentComponent(const ParentComponent&) = default;
		ParentComponent(entt::entity parent);

		operator entt::entity& () { return Parent; }
		operator const entt::entity& () const { return Parent; }

		entt::entity Parent;
	};
}