#pragma once
#include "ComponentsCommonHeaders.h"
#include "Entity.h"
#include "Can/Math.h"

namespace Can::transform
{
	//DEFINE_TYPED_ID(transform_id);
	DEFINE_TYPED_ID(component_id);

	struct init_info
	{
		f32 position[3]{};
		f32 rotation[4]{};
		f32 scale[3]{ 1.0f, 1.0f, 1.0f };
	};

	//class component
	//{
	//public:
	//	explicit component(transform_id id) :_id{ id::invalid_id }, _tramsform{ id } {};
	//	component() = default;
	//	constexpr component_id get_id() const { return _id; }
	//	const bool is_valid() const { return _id != id::invalid_id; }
	//
	//	[[nodiscard]] math::v4 rotation() const;
	//	[[nodiscard]] math::v3 orientation() const;
	//	[[nodiscard]] math::v3 position() const;
	//	[[nodiscard]] math::v3 scale() const;
	//	
	//private:
	//	component_id _id;
	//	transform_id _tramsform{};
	//};

	component create(init_info info, game_entity::entity entity);
	void remove(component c);
	void get_transform_matrices(const game_entity::entity_id id, math::m4x4& world, math::m4x4& inverse_world);
}