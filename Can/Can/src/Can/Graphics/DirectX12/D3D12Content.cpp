#include "canpch.h"
#include "D3D12Content.h"
#include "D3D12Core.h"
#include "D3D12CommonHeaders.h"
#include "D3D12GPass.h"

#include "Can/Utilities/Math.h"
#include "Can/Utilities/IOStream.h"
#include "Can/Content/ContentToEngine.h"


namespace Can::graphics::d3d12::content
{
	namespace
	{
		struct submesh_view
		{
			D3D12_VERTEX_BUFFER_VIEW  position_buffer_view{};
			D3D12_VERTEX_BUFFER_VIEW  element_buffer_view{};
			D3D12_INDEX_BUFFER_VIEW   index_buffer_view{};
			D3D_PRIMITIVE_TOPOLOGY    primitive_topology{};
			u32                       elements_type{};
		};

		std::vector<ID3D12Resource*>          submesh_buffers{}; // Unordered_Array
		std::vector<submesh_view>             submesh_views{};	 // Unordered_Array
		std::mutex                            submesh_mutex{};

		std::vector<d3d12_texture>            textures{};	 // Unordered_Array
		std::mutex                            texture_mutex{};

		std::vector<ID3D12RootSignature*>     root_signatures;
		std::unordered_map<u64, id::id_type>  mtl_rs_map;
		std::vector<std::unique_ptr<u8[]>>    materials; // Unordered_Array
		std::mutex                            material_mutex;

		id::id_type create_root_signature(material_type::type type, shader_flags::flags flags);

		class d3d12_material_stream
		{
		public:
			DISABLE_COPY_AND_MOVE(d3d12_material_stream);
			explicit d3d12_material_stream(u8* const material_buffer)
				:_buffer{ material_buffer }
			{
				initialize();
			}
			explicit d3d12_material_stream(std::unique_ptr<u8[]>& material_buffer, material_init_info info)
			{
				assert(!material_buffer);

				u32 shader_count{ 0 };
				u32 flags{ 0 };
				for (u32 i{ 0 }; i < shader_type::count; ++i)
				{
					if (id::is_valid(info.shader_ids[i]))
					{
						++shader_count;
						flags |= (1 << i);
					}
				}

				assert(shader_count && flags);

				const u32 buffer_size{
					sizeof(material_type::type) +
					sizeof(shader_flags::flags) +
					sizeof(id::id_type) +
					sizeof(u32) +
					sizeof(id:id_type) * shader_count +
					(sizeof(id::id_type) + sizeof(u32)) * info.texture_count
				};

				material_buffer = std::make_unique<u8[]>(buffer_size);
				_buffer = material_buffer.get();
				u8* const buffer{ _buffer };

				*(material_type::type*)buffer = info.type;
				*(shader_flags::flags*)(&buffer[shader_flags_index]) = (shader_flags::flags)flags;
				*(id::id_type*)(&buffer[root_signature_index]) = create_root_signature(info.type, (shader_flags::flags)flags);
				*(u32*)(&buffer[texture_count_index]) = info.texture_count;

				initialize();

				if (info.texture_count)
				{
					memcpy(_texture_ids, info.texture_ids, info.texture_count * size(id::id_type));
					texture::get_descriptor_indices(_texture_ids, info.texture_count, _descriptor_indices);
				}

				u32 shader_index{ 0 };
				for (u32 i{ 0 }; i < shader_type::count; ++i)
				{
					if (id::is_valid(info.shader_ids[i]))
					{
						_shader_ids[shader_index] = info.shader_ids[i];
						++shader_index;
					}
				}

				assert(shader_index == (u32)_mm_popcnt_u32(_shader_flags));
			}
			[[nodiscard]] constexpr u32 texture_count() const { return _texture_count; }
			[[nodiscard]] constexpr material_type::type material_type() const { return _type; }
			[[nodiscard]] constexpr shader_flags::flags shader_flags() const { return _shader_flags; }
			[[nodiscard]] constexpr id::id_type root_signature_id() const { return _root_signature_id; }
			[[nodiscard]] constexpr id::id_type* texture_ids() const { return _texture_ids; }
			[[nodiscard]] constexpr u32* descriptor_indices() const { return _descriptor_indices; }
			[[nodiscard]] constexpr id::id_type* shader_ids() const { return _shader_ids; }
		private:
			void initialize()
			{
				assert(_buffer);
				u8* const buffer{ _buffer };

				_type = *(material_type::type*)buffer;
				_shader_flags = *(shader_flags::flags*)(&buffer[shader_flags_index]);
				_root_signature_id = *(id::id_type*)(&buffer[root_signature_index]);
				_texture_count = *(u32*)(&buffer[texture_count_index]);

				_shader_ids = *(id::id_type*)(&buffer[texture_count_index + sizeof(u32)]);
				_texture_ids = _texture_count ? *(id::id_type*)(&buffer[texture_count_index + sizeof(u32)]) : nullptr;
				_descriptor_indices = _texture_count ? *(id::id_type*)(&buffer[texture_count_index + sizeof(u32)]) : nullptr;
			}

			constexpr static u32 shader_flags_index{ sizeof(material_type::type) };
			constexpr static u32 root_signature_index{ shader_flags_index + sizeof(shader_flags::flags) };
			constexpr static u32 texture_count_index{ root_signature_index + sizeof(id::id_type) };

			u8* _buffer;
			id::id_type* _texture_ids;
			u32* _descriptor_indices;
			id::id_type* _shader_ids;
			id::id_type _root_signature_id;
			u32 _texture_count;
			material_type::type _type;
			shader_flags::flags _shader_flags;
		};

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

		constexpr D3D12_ROOT_SIGNATURE_FLAGS get_root_signature_flags(shader_flags::flags flags)
		{
			D3D12_ROOT_SIGNATURE_FLAGS flags{ d3dx::d3d12_root_signature_desc::default_flags };

			if (flags & shader_flags::vertex)        flags &= ~D3D12_ROOT_SIGNATURE_FLAG_DENY_VERTEX_SHADER_ROOT_ACCESS;
			if (flags & shader_flags::hull)          flags &= ~D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS;
			if (flags & shader_flags::domain)        flags &= ~D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS;
			if (flags & shader_flags::geometry)      flags &= ~D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;
			if (flags & shader_flags::pixel)         flags &= ~D3D12_ROOT_SIGNATURE_FLAG_DENY_PIXEL_SHADER_ROOT_ACCESS;
			if (flags & shader_flags::amplification) flags &= ~D3D12_ROOT_SIGNATURE_FLAG_DENY_AMPLIFICATION_SHADER_ROOT_ACCESS;
			if (flags & shader_flags::mesh)          flags &= ~D3D12_ROOT_SIGNATURE_FLAG_DENY_MESH_SHADER_ROOT_ACCESS;

			return flags;
		}

		id::id_type create_root_signature(material_type::type type, shader_flags::flags flags)
		{
			assert(type < material_type::count);
			static_assert(size(type) == sizeof(u32) && sizeof(flags) == sizeof(u32));
			const u64 key{ ((u64)type << 32) | flags };
			auto pair = mtl_rs_map.find(key);
			if (pair != mtl_rs_map.end())
			{
				assert(pair->first == key);
				return pair->secod;
			}

			ID3D12RootSignature* root_siignature{ nullptr };
			switch (type)
			{
			case material_type::opaque:
			{
				using params = gpass::opaque_root_parameter;
				d3dx::d3d12_root_parameter parameters[params::count]{};
				parameters[params::per_frame_data].as_cbv(D3D12_SHADER_VISIBILITY_ALL, 0);

				D3D12_SHADER_VISIBILITY buffer_visibility{};
				D3D12_SHADER_VISIBILITY data_visibility{};

				if (flags & shader_flags::vertex)
				{
					buffer_visibility = D3D12_SHADER_VISIBILITY_VERTEX;
					data_visibility = D3D12_SHADER_VISIBILITY_VERTEX;
				}
				else if (flags & shader_flags::mesh)
				{
					buffer_visibility = D3D12_SHADER_VISIBILITY_MESH;
					data_visibility = D3D12_SHADER_VISIBILITY_MESH;
				}

				if ((flags & shader_flags::hull) || (flags & shader_flags::geometry) || (flags & shader_flags::amplification))
				{
					buffer_visibility = D3D12_SHADER_VISIBILITY_ALL;
					data_visibility = D3D12_SHADER_VISIBILITY_ALL;
				}

				if ((flags & shader_flags::pixel) || (flags & shader_flags::compute))
				{
					data_visibility = D3D12_SHADER_VISIBILITY_ALL;
				}

				parameters[params::position_buffer].as_srv(buffer_visibility, 0);
				parameters[params::element_buffer].as_srv(buffer_visibility, 1);
				parameters[params::srv_indices].as_srv(D3D12_SHADER_VISIBILITY_PIXEL, 2);
				parameters[params::per_object_data].as_srv(data_visibility, 1);

				root_signature = d3dx::d3d12_root_signature_desc{ parameters, _countof(parameters), get_root_signature_flags(flags) }.create();

				break;
			}
			}

			aseert(root_signature);
			const id::id_type id{ (id::id_type)root_signatures.size() };
			root_signatures.push_back(root_signature);
			mtl_rs_map[key] = id;
			NAME_D3D12_OBJECT_INDEXED(root_signature, key, L"GPass Root Signature - key");
			return id;
		}
	}
	bool initialize()
	{
		return true;
	}
	void shutdown()
	{
		for (auto& item : root_signatures)
		{
			core::release(item);
		}

		mtl_rs_map.clear();
		root_signatures.clear();
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

			submesh_view submesh_view{};
			submesh_view.position_buffer_view.BufferLocation = resource->GetGPUVirtualAddress();
			submesh_view.position_buffer_view.SizeInBytes = position_buffer_size;
			submesh_view.position_buffer_view.StrideInBytes = sizeof(v3);

			submesh_view.index_buffer_view.BufferLocation = resource->GetGPUVirtualAddress() + aligned_position_buffer_size + aligned_element_buffer_size;
			submesh_view.index_buffer_view.Format = (index_size == sizeof(16)) ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT;
			submesh_view.index_buffer_view.SizeInBytes = index_buffer_size;

			if (element_size)
			{
				submesh_view.element_buffer_view.BufferLocation = resource->GetGPUVirtualAddress() + aligned_position_buffer_size;
				submesh_view.element_buffer_view.SizeInBytes = element_buffer_size;
				submesh_view.element_buffer_view.StrideInBytes = element_size;
			}
			submesh_view.elements_type = elements_type;
			submesh_view.primitive_topology = get_d3d_primitive_topology((Can::content::primitive_topology::type)primitive_topology);

			std::lock_guard lock{ submesh_mutex };
			submesh_buffers.push_back(resource);
			submesh_views.push_back(submesh_view);
			return submesh_views.size() - 1;
		}

		void remove(id::id_type id)
		{
			assert(false);
			std::lock_guard lock{ submesh_mutex };
			submesh_views.erase(submesh_views.begin() + id);

			core::deferred_release(submesh_buffers[id]);
			submesh_buffers.erase(submesh_buffers.begin() + id);
		}
	}

	namespace material
	{
		// NOTE: Expects 'buffer' to contain:
		//  ...
		//  ...
		//  ...
		// shader_flags::flags flags
		// id::id_type         root_signature_id
		// u32                 texture_count
		// id::id_type         shader_ids[shader_count]
		// id::id_type         texture_ids[texture_count]
		// u32*                descriptor_indices[texture_count]
		id::id_type add(material_init_info info)
		{
			std::unique_ptr<u8[]> buffer;
			std::lock_guard lock{ material_mutex };
			d3d12_material_stream stream{ buffer, info };
			assert(buffer);
			materials.push_back(std::move(buffer));
			return materials.size() - 1;
		}

		void remove(id::id_type id)
		{
			std::lock_guard lock{ material_mutex };
			materials.erase(materials.begin() + id); // Unordered_Array
		}
	}
}