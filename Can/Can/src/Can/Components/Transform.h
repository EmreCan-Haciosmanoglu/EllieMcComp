#pragma once
#include "ComponentsCommonHeaders.h"
#include "Entity.h"
#include "Can\Math.h"

namespace Can::transform
{
	DEFINE_TYPED_ID(transform_id);
	DEFINE_TYPED_ID(component_id);

	struct init_info
	{
		Math::Vector4<f32> rotation{ 0.0f };
		Math::Vector3<f32> position{ 0.0f };
		Math::Vector3<f32> scale{ 1.0f, 1.0f, 1.0f };
	};

	class component
	{
	public:
		explicit component(transform_id id) :_id{ id::invalid_id }, _tramsform{ id } {};
		component() = default;
		constexpr component_id get_id() const { return _id; }
		const bool is_valid() const { return _id != id::invalid_id; }

		[[nodiscard]] Math::Vector4<f32> rotation() const;
		[[nodiscard]] Math::Vector3<f32> position() const;
		[[nodiscard]] Math::Vector3<f32> scale() const;
	private:
		component_id _id;
		transform_id _tramsform{};
	};

	component create(init_info info, game_entity::entity entity);
	void remove(component c);
}