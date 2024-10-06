#pragma once
#include "ComponentsCommonHeaders.h"

namespace Can::transform
{
	DEFINE_TYPED_ID(transform_id);

	struct init_info
	{
		f32 position[3]{};
		f32 rotation[4]{};
		f32 scale[3]{ 1.0f, 1.0f, 1.0f };
	};

	transform_id create_transform(const init_info& info, game_entity::entity_id entity_id);
	transform_id remove_transform(transform_id id);
}