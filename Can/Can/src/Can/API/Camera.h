#pragma once
#include "Can/Common/CommonHeaders.h"

namespace Can::graphics
{
	DEFINE_TYPED_ID(camera_id);

	class camera
	{
	public:
		enum type : u32
		{
			perspective,
			orthographic
		};

		constexpr explicit camera(camera_id id) : _id{ id } {}
		constexpr camera() : _id{ id::invalid_id } {}
		constexpr camera_id get_id() const { return _id; }
		constexpr bool is_valid() const { return id::is_valid(_id); }

		void up(v3 up) const;
		void field_of_view(f32 fov) const;
		void aspect_ratio(f32 aspect_ratio) const;
		void view_width(f32 width) const;
		void view_height(f32 height) const;
		void range(f32 near_z, f32 far_z) const;

		m4 view() const;
		m4 projection() const;
		m4 inverse_projection() const;
		m4 view_projection() const;
		m4 inverse_view_projection() const;
		v3 up() const;
		f32 near_z() const;
		f32 far_z() const;
		f32 field_of_view() const;
		f32 aspect_ratio() const;
		f32 view_width() const;
		f32 view_height() const;
		type projection_type() const;
		id::id_type entity_id() const;
	private:
		camera_id _id; 
	};
}