#include "canpch.h"
#include "D3D12LightCulling.h"
#include "D3D12Core.h"
#include "Shaders/SharedTypes.h"
#include "D3D12Shaders.h"
#include "D3D12Light.h"
#include "D3D12Camera.h"
#include "D3D12GPass.h"

namespace Can::graphics::d3d12::delight
{
	namespace
	{
		struct light_culling_root_parameter
		{
			enum parameter : u32
			{
				global_shader_data,
				constants,
				frustums_out_or_index_counter,
				frustums_in,
				culling_info,
				light_grid_opaque,
				light_index_list_opaque,

				count
			};
		};

		struct culling_parameters
		{
			d3d12_buffer                         frustums;
			d3d12_buffer                         light_grid_and_index_list;
			structured_buffer                    light_index_counter;
			hlsl::LightCullingDispacthParameters grid_frustums_dispatch_params{};
			hlsl::LightCullingDispacthParameters light_culling_dispatch_params{};
			u32                                  frustum_count{ 0 };
			u32                                  view_width{ 0 };
			u32                                  view_height{ 0 };
			f32                                  camera_fov{ 0 };
			D3D12_GPU_VIRTUAL_ADDRESS            light_index_list_opaque_buffer{ 0 };
			bool                                 has_lights{ true };
		};

		struct light_culler
		{
			culling_parameters cullers[frame_buffer_count]{};
		};

		constexpr u32        max_lights_per_tile{ 256 };

		ID3D12RootSignature* light_culling_root_signature{ nullptr };
		ID3D12PipelineState* grid_frustum_pso{ nullptr };
		ID3D12PipelineState* light_culling_pso{ nullptr };
		utl::vector<light_culler> light_cullers; // TODO: Unordered_Array

		bool create_root_signatures()
		{
			assert(!light_culling_root_signature);
			using params = light_culling_root_parameter;
			d3dx::d3d12_root_parameter parameters[params::count]{};
			parameters[params::global_shader_data].as_cbv(D3D12_SHADER_VISIBILITY_ALL, 0);
			parameters[params::constants].as_cbv(D3D12_SHADER_VISIBILITY_ALL, 1);
			parameters[params::frustums_out_or_index_counter].as_uav(D3D12_SHADER_VISIBILITY_ALL, 0);
			parameters[params::frustums_in].as_srv(D3D12_SHADER_VISIBILITY_ALL, 0);
			parameters[params::culling_info].as_srv(D3D12_SHADER_VISIBILITY_ALL, 1);
			parameters[params::light_grid_opaque].as_uav(D3D12_SHADER_VISIBILITY_ALL, 1);
			parameters[params::light_index_list_opaque].as_uav(D3D12_SHADER_VISIBILITY_ALL, 3);

			light_culling_root_signature = d3dx::d3d12_root_signature_desc{ parameters, _countof(parameters) }.create();
			NAME_D3D12_OBJECT(light_culling_root_signature, L"Light Culling Root Signature");

			return light_culling_root_signature != nullptr;
		}

		bool create_psos()
		{
			{
				assert(!grid_frustum_pso);
				struct {
					d3dx::d3d12_pipeline_state_subobject_root_signature root_signature{ light_culling_root_signature };
					d3dx::d3d12_pipeline_state_subobject_cs cs{ shaders::get_engine_shader(shaders::engine_shader::grid_frustums_cs) };

				} stream;

				grid_frustum_pso = d3dx::create_pipeline_state(&stream, sizeof(stream));
				NAME_D3D12_OBJECT(grid_frustum_pso, L"Grid Frustums PSO");
			}
			{
				assert(!light_culling_pso);
				struct {
					d3dx::d3d12_pipeline_state_subobject_root_signature root_signature{ light_culling_root_signature };
					d3dx::d3d12_pipeline_state_subobject_cs cs{ shaders::get_engine_shader(shaders::engine_shader::light_culling_cs) };

				} stream;

				light_culling_pso = d3dx::create_pipeline_state(&stream, sizeof(stream));
				NAME_D3D12_OBJECT(light_culling_pso, L"Light Culling PSO");
			}

			return grid_frustum_pso != nullptr && light_culling_pso != nullptr;
		}

		void resize_buffers(culling_parameters& culler)
		{
			const u32 frustum_count{ culler.frustum_count };

			const u32 frustum_buffer_size{ sizeof(hlsl::Frustum) * frustum_count };
			const u32 light_grid_buffer_size{ (u32)math::align_size_up<sizeof(math::v4)>(sizeof(math::u32v2) * frustum_count) };
			const u32 light_index_list_buffer_size{ (u32)math::align_size_up<sizeof(math::v4)>(sizeof(u32) * max_lights_per_tile * frustum_count) };
			const u32 light_grid_and_index_list_buffer_size{ light_grid_buffer_size + light_index_list_buffer_size };

			d3d12_buffer_init_info info{};
			info.alignment = sizeof(math::v4);
			info.flags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;

			if (frustum_buffer_size > culler.frustums.size())
			{
				info.size = frustum_buffer_size;
				culler.frustums = d3d12_buffer{ info, false };
				NAME_D3D12_OBJECT_INDEXED(culler.frustums.buffer(), frustum_count, L"Light Grid Frustums Buffer - count");
			}

			if (light_grid_and_index_list_buffer_size > culler.light_grid_and_index_list.size())
			{
				info.size = light_grid_and_index_list_buffer_size;
				culler.light_grid_and_index_list = d3d12_buffer{ info, false };

				const D3D12_GPU_VIRTUAL_ADDRESS light_grid_opaque_buffer{ culler.light_grid_and_index_list.gpu_address() };
				culler.light_index_list_opaque_buffer = light_grid_opaque_buffer + light_grid_buffer_size;
				NAME_D3D12_OBJECT_INDEXED(culler.light_grid_and_index_list.buffer(), light_grid_and_index_list_buffer_size, L"Light Grid and Index List Buffer - size");

				if (!culler.light_index_counter.buffer())
				{
					info = structured_buffer::get_default_init_info(sizeof(math::u32v4), 1);
					info.create_uav = true;
					culler.light_index_counter = structured_buffer{ info };
					NAME_D3D12_OBJECT_INDEXED(culler.light_index_counter.buffer(), core::current_frame_index(), L"Light Index Counter Buffer");
				}
			}
		}

		void resize(culling_parameters& culler)
		{
			constexpr u32 tile_size{ light_culling_tile_size };
			assert(culler.view_width >= tile_size && culler.view_height >= tile_size);
			const math::u32v2 tile_count
			{
				(u32)math::align_size_up<tile_size>(culler.view_width) / tile_size,
				(u32)math::align_size_up<tile_size>(culler.view_height) / tile_size
			};

			culler.frustum_count = tile_count.x * tile_count.y;

			{
				hlsl::LightCullingDispacthParameters& params{ culler.grid_frustums_dispatch_params };
				params.NumThreads = tile_count;
				params.NumThreadGroups.x = (u32)math::align_size_up<tile_size>(tile_count.x) / tile_size;
				params.NumThreadGroups.y = (u32)math::align_size_up<tile_size>(tile_count.y) / tile_size;
			}

			{
				hlsl::LightCullingDispacthParameters& params{ culler.light_culling_dispatch_params };
				params.NumThreads.x = tile_count.x * tile_size;
				params.NumThreads.y = tile_count.y * tile_size;
				params.NumThreadGroups = tile_count;
			}

			resize_buffers(culler);
		}

		void calculate_grid_frustums(
			culling_parameters& culler,
			id3d12_graphics_command_list* const cmd_list,
			const d3d12_frame_info& d3d12_info,
			d3dx::d3d12_resource_barrier& barriers
		) {
			constant_buffer& cbuffer{ core::cbuffer() };
			hlsl::LightCullingDispacthParameters* const buffer{ cbuffer.allocate<hlsl::LightCullingDispacthParameters>() };
			const hlsl::LightCullingDispacthParameters& params{ culler.grid_frustums_dispatch_params };
			memcpy(buffer, &params, sizeof(hlsl::LightCullingDispacthParameters));

			barriers.add(
				culler.frustums.buffer(),
				D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE | D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
				D3D12_RESOURCE_STATE_UNORDERED_ACCESS
			);
			barriers.apply(cmd_list);

			using light_params = light_culling_root_parameter;
			cmd_list->SetComputeRootSignature(light_culling_root_signature);
			cmd_list->SetPipelineState(grid_frustum_pso);
			cmd_list->SetComputeRootConstantBufferView(light_params::global_shader_data, d3d12_info.global_shader_data);
			cmd_list->SetComputeRootConstantBufferView(light_params::constants, cbuffer.gpu_address(buffer));
			cmd_list->SetComputeRootUnorderedAccessView(light_params::frustums_out_or_index_counter, culler.frustums.gpu_address());
			cmd_list->Dispatch(params.NumThreadGroups.x, params.NumThreadGroups.y, 1);

			barriers.add(
				culler.frustums.buffer(),
				D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
				D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE | D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE
			);
		}

		void  _declspec(noinline) resize_and_calculate_grid_frustums(
			culling_parameters& culler,
			id3d12_graphics_command_list* const cmd_list,
			const d3d12_frame_info& d3d12_info,
			d3dx::d3d12_resource_barrier& barriers
		) {
			culler.camera_fov = d3d12_info.camera->field_of_view();
			culler.view_width = d3d12_info.surface_width;
			culler.view_height = d3d12_info.surface_height;

			resize(culler);
			calculate_grid_frustums(culler, cmd_list, d3d12_info, barriers);
		}
	}

	bool initialize()
	{
		return create_root_signatures() && create_psos() && light::initialize();
	}

	void shutdown()
	{
		light::shutdown();
		assert(light_culling_root_signature && grid_frustum_pso && light_culling_pso);
		core::deferred_release(light_culling_root_signature);
		core::deferred_release(grid_frustum_pso);
		core::deferred_release(light_culling_pso);
	}

	id::id_type add_culler()
	{
		light_cullers.emplace_back();
		return light_cullers.size() - 1;
	}

	void remove_culler(id::id_type id)
	{
		assert(id::is_valid(id));

		assert(false && "free_list");
		light_cullers.erase(light_cullers.begin() + id);
	}

	void cull_lights(id3d12_graphics_command_list* const cmd_list, const d3d12_frame_info& d3d12_info, d3dx::d3d12_resource_barrier& barriers)
	{
		const id::id_type id{ d3d12_info.light_culling_id };
		assert(id::is_valid(id));
		culling_parameters& culler{ light_cullers[id].cullers[d3d12_info.frame_index] };

		if (d3d12_info.surface_width != culler.view_width || d3d12_info.surface_height != culler.view_height || !math::is_equal(d3d12_info.camera->field_of_view(), culler.camera_fov))
		{
			resize_and_calculate_grid_frustums(culler, cmd_list, d3d12_info, barriers);
		}

		hlsl::LightCullingDispacthParameters& params{ culler.light_culling_dispatch_params };
		params.NumLights = light::cullable_light_count(d3d12_info.info->light_set_key);
		params.DepthBufferSrvIndex = gpass::depth_buffer().srv().index;

		if (!params.NumLights && !culler.has_lights) return;

		culler.has_lights = params.NumLights > 0;

		constant_buffer& cbuffer{ core::cbuffer() };
		hlsl::LightCullingDispacthParameters* const buffer{ cbuffer.allocate<hlsl::LightCullingDispacthParameters>() };
		memcpy(buffer, &params, sizeof(hlsl::LightCullingDispacthParameters));

		barriers.add(
			culler.light_grid_and_index_list.buffer(),
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
			D3D12_RESOURCE_STATE_UNORDERED_ACCESS
		);
		barriers.apply(cmd_list);

		const math::u32v4 clear_value{ 0, 0, 0, 0 };
		culler.light_index_counter.clear_uav(cmd_list, &clear_value.x);

		cmd_list->SetComputeRootSignature(light_culling_root_signature);
		cmd_list->SetPipelineState(light_culling_pso);
		using light_params = light_culling_root_parameter;
		cmd_list->SetComputeRootConstantBufferView(light_params::global_shader_data, d3d12_info.global_shader_data);
		cmd_list->SetComputeRootConstantBufferView(light_params::constants, cbuffer.gpu_address(buffer));
		cmd_list->SetComputeRootUnorderedAccessView(light_params::frustums_out_or_index_counter, culler.light_index_counter.gpu_address());
		cmd_list->SetComputeRootShaderResourceView(light_params::frustums_in, culler.frustums.gpu_address());
		cmd_list->SetComputeRootShaderResourceView(light_params::culling_info, light::culling_info_buffer(d3d12_info.frame_index));
		cmd_list->SetComputeRootUnorderedAccessView(light_params::light_grid_opaque, culler.light_grid_and_index_list.gpu_address());
		cmd_list->SetComputeRootUnorderedAccessView(light_params::light_index_list_opaque, culler.light_index_list_opaque_buffer);

		cmd_list->Dispatch(params.NumThreadGroups.x, params.NumThreadGroups.y, 1);

		barriers.add(
			culler.light_grid_and_index_list.buffer(),
			D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE
		);
	}

	// TODO: Temporary for visualization. Remove later
	D3D12_GPU_VIRTUAL_ADDRESS frustums(id::id_type light_culling_id, u32 frame_index)
	{
		assert(frame_index < frame_buffer_count && id::is_valid(light_culling_id));
		return light_cullers[light_culling_id].cullers[frame_index].frustums.gpu_address();
	}
}