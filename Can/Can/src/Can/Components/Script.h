#pragma once
#include "ComponentsCommonHeaders.h"
#include "Entity.h"

namespace Can::script
{
	struct init_info
	{
		detail::script_creator script_creator;
	};

	component create(init_info info, game_entity::entity entity);
	void remove(component c);
	void update(f32 dt);
} 