#include "canpch.h"
#include "Renderer.h"
#include "GraphicsPlatformInterface.h"
#include "DirectX12/D3D12Interface.h"

namespace Can::graphics
{
	namespace
	{
		constexpr const char* engine_shader_paths[]
		{
			".\\shaders\\d3d12\\shaders.bin",
			//".\\shaders\\vulkan\\shaders.bin", etc.
		};

		platform_interface gfx{};

		bool set_platform_interface(graphics_platform platform, platform_interface& pi)
		{
			switch (platform)
			{
			case Can::graphics::graphics_platform::direct3d12:
			{
				d3d12::get_platform_interface(pi);
				break;
			}
			default:
				return false;
				break;
			}

			assert(pi.platform == platform);
			return true;
		}
	}

	bool initialize(graphics_platform platform)
	{
		return set_platform_interface(platform, gfx) && gfx.initialize();
	}

	void shutdown()
	{
		if (gfx.platform != (graphics_platform)-1) gfx.shutdown();
	}
 
	const char* get_engine_shaders_path()
	{
		return engine_shader_paths[(u32)gfx.platform];
	}

	const char* get_engine_shaders_path(graphics_platform platform)
	{
		return engine_shader_paths[(u32)platform];
	}

	surface create_surface(platform::window window)
	{
		return gfx.surface.create(window);
	}

	void remove_surface(surface_id id)
	{
		assert(id::is_valid(id));
		gfx.surface.remove(id);
	}

	void surface::resize(u32 width, u32 height) const
	{
		assert(is_valid());
		gfx.surface.resize(_id, width, height);
	}

	u32 surface::width() const
	{
		assert(is_valid());
		return gfx.surface.width(_id);
	}

	u32 surface::height() const
	{
		assert(is_valid());
		return gfx.surface.height(_id);
	}

	void surface::render(frame_info info) const
	{
		assert(is_valid());
		gfx.surface.render(_id, info);
	}

	camera create_camera(camera_init_info info)
	{
		return gfx.camera.create(info);
	}

	void remove_camera(camera_id id)
	{
		gfx.camera.remove(id);
	}

	void camera::up(v3 up) const
	{
		assert(is_valid());
		gfx.camera.set_parameter(_id, camera_parameter::up_vector, &up, sizeof(up));
	}
	void camera::field_of_view(f32 fov) const
	{
		assert(is_valid());
		gfx.camera.set_parameter(_id, camera_parameter::field_of_view, &fov, sizeof(fov));
	}
	void camera::aspect_ratio(f32 aspect_ratio) const
	{
		assert(is_valid());
		gfx.camera.set_parameter(_id, camera_parameter::aspect_ratio, &aspect_ratio, sizeof(aspect_ratio));
	}
	void camera::view_width(f32 width) const
	{
		assert(is_valid());
		gfx.camera.set_parameter(_id, camera_parameter::view_width, &width, sizeof(width));
	}
	void camera::view_height(f32 height) const
	{
		assert(is_valid());
		gfx.camera.set_parameter(_id, camera_parameter::view_height, &height, sizeof(height));
	}
	void camera::range(f32 near_z, f32 far_z) const
	{
		assert(is_valid());
		gfx.camera.set_parameter(_id, camera_parameter::near_z, &near_z, sizeof(near_z));
		gfx.camera.set_parameter(_id, camera_parameter::far_z, &far_z, sizeof(far_z));
	}

	m4 camera::view() const
	{
		assert(is_valid());
		m4 matrix{};
		gfx.camera.get_parameter(_id, camera_parameter::view, &matrix, sizeof(matrix));
		return matrix;
	}
	m4 camera::projection() const
	{
		assert(is_valid());
		m4 matrix{};
		gfx.camera.get_parameter(_id, camera_parameter::projection, &matrix, sizeof(matrix));
		return matrix;
	}
	m4 camera::inverse_projection() const
	{
		assert(is_valid());
		m4 matrix{};
		gfx.camera.get_parameter(_id, camera_parameter::inverse_projection, &matrix, sizeof(matrix));
		return matrix;
	}
	m4 camera::view_projection() const
	{
		assert(is_valid());
		m4 matrix{};
		gfx.camera.get_parameter(_id, camera_parameter::view_projection, &matrix, sizeof(matrix));
		return matrix;
	}
	m4 camera::inverse_view_projection() const
	{
		assert(is_valid());
		m4 matrix{};
		gfx.camera.get_parameter(_id, camera_parameter::inverse_view_projection, &matrix, sizeof(matrix));
		return matrix;
	}
	v3 camera::up() const
	{
		assert(is_valid());
		v3 up_vector{};
		gfx.camera.get_parameter(_id, camera_parameter::up_vector, &up_vector, sizeof(up_vector));
		return up_vector;
	}
	f32 camera::near_z() const 
	{
		assert(is_valid());
		f32 near_z{};
		gfx.camera.get_parameter(_id, camera_parameter::near_z, &near_z, sizeof(near_z));
		return near_z;
	}
	f32 camera::far_z() const
	{
		assert(is_valid());
		f32 far_z{};
		gfx.camera.get_parameter(_id, camera_parameter::far_z, &far_z, sizeof(far_z));
		return far_z;
	}
	f32 camera::field_of_view() const
	{
		assert(is_valid());
		f32 field_of_view{};
		gfx.camera.get_parameter(_id, camera_parameter::field_of_view, &field_of_view, sizeof(field_of_view));
		return field_of_view;
	}
	f32 camera::aspect_ratio() const
	{
		assert(is_valid());
		f32 aspect_ratio{};
		gfx.camera.get_parameter(_id, camera_parameter::aspect_ratio, &aspect_ratio, sizeof(aspect_ratio));
		return aspect_ratio;
	}
	f32 camera::view_width() const
	{
		assert(is_valid());
		f32 view_width{};
		gfx.camera.get_parameter(_id, camera_parameter::view_width, &view_width, sizeof(view_width));
		return view_width;
	}
	f32 camera::view_height() const
	{
		assert(is_valid());
		f32 view_height{};
		gfx.camera.get_parameter(_id, camera_parameter::view_height, &view_height, sizeof(view_height));
		return view_height;
	}
	camera::type camera::projection_type() const
	{
		assert(is_valid());
		camera::type projection_type{};
		gfx.camera.get_parameter(_id, camera_parameter::type, &projection_type, sizeof(projection_type));
		return projection_type;
	}
	id::id_type camera::entity_id() const
	{
		assert(is_valid());
		id::id_type entity_id{};
		gfx.camera.get_parameter(_id, camera_parameter::entity_id, &entity_id, sizeof(entity_id));
		return entity_id;
	}

	id::id_type add_submesh(const u8*& data)
	{
		return gfx.resources.add_submesh(data);
	}

	void remove_submesh(id::id_type id)
	{
		gfx.resources.remove_submesh(id);
	}

	id::id_type add_material(material_init_info info)
	{
		return gfx.resources.add_material(info);
	}

	void remove_material(id::id_type id)
	{
		gfx.resources.remove_material(id);
	}

	id::id_type add_render_item(id::id_type entity_id, id::id_type geometry_content_id, u32 material_count, const id::id_type* const material_ids)
	{
		return gfx.resources.add_render_item(entity_id, geometry_content_id, material_count, material_ids);
	}
	void remove_render_item(id::id_type id)
	{
		gfx.resources.remove_render_item(id);
	}
}