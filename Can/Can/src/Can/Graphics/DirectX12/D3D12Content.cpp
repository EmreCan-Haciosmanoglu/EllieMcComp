#include "canpch.h"
#include "D3D12Content.h"
#include "D3D12Core.h"
#include "Can\Utilities\Math.h"
#include "Can\Content\ContentToEngine.h"


namespace Can::graphics::d3d12::content
{
	namespace
	{
		struct position_view
		{
			D3D12_VERTEX_BUFFER_VIEW  position_buffer_view{};
			D3D12_INDEX_BUFFER_VIEW   index_buffer_view{};
		};

		struct element_view
		{
			D3D12_VERTEX_BUFFER_VIEW  element_buffer_view{};
			u32                       elements_type{};
			D3D_PRIMITIVE_TOPOLOGY    primitive_topology{};
		};

		std::vector<ID3D12Resource*>  submesh_buffers{};
		std::vector<position_view>    position_views{};
		std::vector<element_view>     element_views{};
		std::mutex                    submesh_mutex{};

		D3D_PRIMITIVE_TOPOLOGY get_d3d_primitive_topology(Can::content::primitive_topology::type type)
		{
			assert(type < Can::content::primitive_topology::count);

			switch (type)
			{
			case Can::content::primitive_topology::point_List:
				return D3D_PRIMITIVE_TOPOLOGY_POINTLIST;
			case Can::content::primitive_topology::line_list:
				return D3D_PRIMITIVE_TOPOLOGY_LINELIST;
			case Can::content::primitive_topology::line_strip:
				return D3D_PRIMITIVE_TOPOLOGY_LINESTRIP;
			case Can::content::primitive_topology::triangle_list:
				return D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
			case Can::content::primitive_topology::triangle_strip:
				return D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
			}
			return D3D_PRIMITIVE_TOPOLOGY_UNDEFINED;
		}
	}

	namespace submesh
	{
		// NOTE: Expects 'data' to contain:
		// u32 element_size
		// u32 vertex_count
		// u32 index_count
		// u32 elements_type
		// u32 primitive_topology
		// u8 positions[sizeof(f32) * 3 * vertex_count]
		// u8 elements[element_size * vertex_count]
		// u8 indices[index_size * index_count]
		// 
		// Remarks:
		// -Advances the data pointer 
		// -Position and element buffers shhould be padded to be a multiple of 4 bytes in length. 

		id::id_type add(const u8*& data)
		{
			utl::blob_stream_reader blob{ (const u8*)data };

			const u32 element_size{ blob.read<u32>() };
			const u32 vertex_count{ blob.read<u32>() };
			const u32 index_count{ blob.read<u32>() };
			const u32 elements_type{ blob.read<u32>() };
			const u32 primitive_topology{ blob.read<u32>() };
			const u32 index_size{ (vertex_count < (1 << 16)) ? sizeof(16) : sizeof(u32) };

			const u32 position_buffer_size{ sizeof(v3) * vertex_count };
			const u32 element_buffer_size{ element_size * vertex_count };
			const u32 index_buffer_size{ index_size * index_count };

			constexpr u32 alignment{ D3D12_STANDARD_MAXIMUM_ELEMENT_ALIGNMENT_BYTE_MULTIPLE };
			const u32 aligned_position_buffer_size{ (u32)math::align_size_up<alignment>(position_buffer_size) };
			const u32 aligned_element_buffer_size{ (u32)math::align_size_up<alignment>(element_buffer_size) };
			const u32 total_buffer_size{ aligned_position_buffer_size + aligned_element_buffer_size + index_buffer_size };

			ID3D12Resource* resource{ d3dx::create_buffer((void*)blob.position(), total_buffer_size) };

			blob.skip(total_buffer_size);
			data = blob.position();

			position_view position_view{};
			position_view.position_buffer_view.BufferLocation = resource->GetGPUVirtualAddress();
			position_view.position_buffer_view.SizeInBytes = position_buffer_size;
			position_view.position_buffer_view.StrideInBytes = sizeof(v3);

			position_view.index_buffer_view.BufferLocation = resource->GetGPUVirtualAddress() + aligned_position_buffer_size + aligned_element_buffer_size;
			position_view.index_buffer_view.Format = (index_size == sizeof(16)) ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT;
			position_view.index_buffer_view.SizeInBytes = index_buffer_size;


			element_view element_view{};
			if (element_size)
			{
				element_view.element_buffer_view.BufferLocation = resource->GetGPUVirtualAddress() + aligned_position_buffer_size;
				element_view.element_buffer_view.SizeInBytes = element_buffer_size;
				element_view.element_buffer_view.StrideInBytes = element_size;
			}
			element_view.elements_type = elements_type;
			element_view.primitive_topology = get_d3d_primitive_topology((Can::content::primitive_topology::type)primitive_topology);

			std::lock_guard lock{ submesh_mutex };
			submesh_buffers.push_back(resource);
			position_views.push_back(position_view);
			element_views.push_back(element_view);
			return element_views.size() - 1;
		}

		void remove(id::id_type id)
		{
			std::lock_guard lock{ submesh_mutex };
			position_views.erase(position_views.begin() + id);
			element_views.erase(element_views.begin() + id);

			core::deferred_release(submesh_buffers[id]);
			submesh_buffers.erase(submesh_buffers.begin() + id);
		}
	}
}