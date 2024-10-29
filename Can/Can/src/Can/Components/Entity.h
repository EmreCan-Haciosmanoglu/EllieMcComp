#pragma once
#include "ComponentsCommonHeaders.h"

namespace Can
{
#define INIT_INFO(component) namespace component { struct init_info; }

INIT_INFO(transform)
INIT_INFO(script)

#undef INIT_INFO

	namespace game_entity
	{
		//DEFINE_TYPED_ID(entity_id);

		struct entity_info
		{
			transform::init_info* transform{ nullptr };
			script::init_info* script{ nullptr };
		};

		//struct entity 
		//{
		//	constexpr entity_id get_id() const { return _id; }
		//	const bool is_valid() const { return _id != id::invalid_id; }
		//	entity_id _id;
		//};

		bool is_alive(entity_id id);

		void remove(entity_id id);
		entity create(const entity_info& info);
	}
}