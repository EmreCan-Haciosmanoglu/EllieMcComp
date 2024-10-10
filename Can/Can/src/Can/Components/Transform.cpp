#include "canpch.h"
#include "Transform.h"
#include "Can\Unordered_Array.h"

namespace Can::transform
{
	namespace
	{
		Unordered_Array<Math::Vector4<f32>> rotations;
		Unordered_Array<Math::Vector3<f32>> positions;
		Unordered_Array<Math::Vector3<f32>> scales;
	}

	component create(init_info info, game_entity::entity entity)
	{
		assert(entity.is_valid());
		const id::id_type entity_index{ id::index(entity.get_id()) };

		if (positions.size > entity_index)
		{
			rotations[entity_index] = info.rotation;
			positions[entity_index] = info.position;
			scales[entity_index] = info.scale;
		}
		else
		{
			assert(positions.size == entity_index);
			array_add(&rotations, &info.rotation);
			array_add(&positions, &info.position);
			array_add(&scales, &info.scale);
		}

		return component{ transform_id{ entity_index } };
	}

	void remove([[maybe_unused]] component c)
	{
		assert(c.is_valid());
	}
	Math::Vector4<f32> component::rotation() const
	{
		assert(is_valid());
		return rotations[id::index(_id)];
	}
	Math::Vector3<f32> component::position() const
	{
		assert(is_valid());
		return positions[id::index(_id)];
	}
	Math::Vector3<f32> component::scale() const
	{
		assert(is_valid());
		return scales[id::index(_id)];
	}
}