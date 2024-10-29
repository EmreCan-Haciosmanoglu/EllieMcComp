#pragma once
#include "ComponentsCommonHeaders.h"
#include "Entity.h"

namespace Can::script
{
	//DEFINE_TYPED_ID(script_id);

	struct init_info
	{
		f32 p{ 0.0f };
	};

	script_id create_transform(const init_info& info, game_entity::entity_id entity_id);
	script_id remove_transform(script_id id);
}