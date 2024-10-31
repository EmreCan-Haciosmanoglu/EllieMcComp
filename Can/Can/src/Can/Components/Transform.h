#pragma once
#include "ComponentsCommonHeaders.h"
#include "Entity.h"
#include "Can/Math.h"

namespace Can::transform
{
	struct init_info
	{
		f32 position[3]{};
		f32 rotation[4]{};
		f32 scale[3]{ 1.0f, 1.0f, 1.0f };
	};

	component create(init_info info, game_entity::entity entity);
	void remove(component c);
	void get_transform_matrices(const game_entity::entity_id id, math::m4x4& world, math::m4x4& inverse_world);
}