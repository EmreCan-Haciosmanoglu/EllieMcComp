#include "canpch.h"
#include "ContentToEngine.h"
#include "Can/Common/Id.h"
#include "Can/Utilities/IOStream.h"

#include "Can/Graphics/Renderer.h"

namespace Can::content
{
	namespace
	{
		class geometry_hierarchy_stream
		{
		public:

			DISABLE_COPY_AND_MOVE(geometry_hierarchy_stream);
			geometry_hierarchy_stream(u8* const buffer, u32 lods = u32_invalid_id)
				:_buffer{ buffer }
			{
				assert(buffer && lods);
				if (lods != u32_invalid_id)
				{
					*((u32*)buffer) = lods;
				}

				_lod_count = *((u32*)buffer);
				_thresholds = (f32*)(&buffer[sizeof(u32)]);
				_lod_offsets = (lod_offset*)(&_thresholds[_lod_count]);
				_gpu_ids = (id::id_type*)(&_lod_offsets[_lod_count]);
			}

			void gpu_ids(u32 lod, id::id_type*& ids, u32& id_count)
			{
				assert(lod < _lod_count);
				ids = &_gpu_ids[_lod_offsets[lod].offset];
				id_count = _lod_offsets[lod].count;
			}

			u32 lod_from_threshold(f32 threshold)
			{
				assert(threshold > 0);
				if (_lod_count == 0) return 0;

				for (u32 i{ _lod_count - 1 }; i > 0; --i)
				{
					if (_thresholds[i] <= threshold) return i;
				}

				assert(false);
				return 0;
			}

			[[nodiscard]] constexpr u32 lod_count() const { return _lod_count; }
			[[nodiscard]] constexpr f32* thresholds() const { return _thresholds; }
			[[nodiscard]] constexpr lod_offset* lod_offsets() const { return _lod_offsets; }
			[[nodiscard]] constexpr id::id_type* gpu_ids() const { return _gpu_ids; }

		private:
			u8* const _buffer;
			f32* _thresholds;
			lod_offset* _lod_offsets;
			id::id_type* _gpu_ids;
			u32 _lod_count;
		};

		constexpr uintptr_t  single_mesh_marker{ (uintptr_t)0x01 };
		std::vector<u8*>     geometry_hierarchies; // Unordered_Array
		std::mutex           geometry_mutex;

		std::vector<std::unique_ptr<u8[]>> shaders;
		std::mutex                         shader_mutex; // Unordered_Array

		u32 get_geometry_hierarchy_buffer_size(const void* const data)
		{
			assert(data);
			utl::blob_stream_reader blob{ (const u8*)data };
			const u32 lod_count{ blob.read<u32>() };
			assert(lod_count);
			u32 size{ sizeof(u32) + (sizeof(f32) + sizeof(lod_offset)) * lod_count };

			for (u32 lod_idx{ 0 }; lod_idx < lod_count; ++lod_idx)
			{
				blob.skip(sizeof(f32));
				size += sizeof(id::id_type) + blob.read<u32>();
				blob.skip(blob.read<u32>());
			}

			return size;
		}

		id::id_type create_mesh_hierarchy(const void* const data)
		{
			assert(data);
			const u32 size{ get_geometry_hierarchy_buffer_size(data) };
			u8* const hierarchy_buffer{ (u8* const)malloc(size) };

			utl::blob_stream_reader blob{ (const u8*)data };
			const u32 lod_count{ blob.read<u32>() };
			assert(lod_count);
			geometry_hierarchy_stream stream{ hierarchy_buffer, lod_count };
			u32 submesh_index{ 0 };
			id::id_type* const gpu_ids{ stream.gpu_ids() };

			for (u32 lod_idx{ 0 }; lod_idx < lod_count; ++lod_idx)
			{
				stream.thresholds()[lod_idx] = blob.read<f32>();
				const u32 id_count{ blob.read<u32>() };
				assert(id_count < (1 << 16));
				stream.lod_offsets()[lod_idx] = { (u16)submesh_index, (u16)id_count };
				blob.skip(sizeof(u32));

				for (u32 id_idx{ 0 }; id_idx < id_count; ++id_idx)
				{
					const u8* at{ blob.position() };
					gpu_ids[submesh_index++] = graphics::add_submesh(at);
					blob.skip((u32)(at - blob.position()));
					assert(submesh_index < (1 << 16));
				}
			}

			assert([&]() {
				f32 previous_threshold{ stream.thresholds()[0] };
				for (u32 i{ 0 }; i < lod_count; ++i)
				{
					if (stream.thresholds()[i] <= previous_threshold) return false;
					previous_threshold = stream.thresholds()[i];
				}
				return true;
				}());

			static_assert(alignof(void*) > 2, "We need the least significant bit for the single mesh marker.");
			std::lock_guard lock{ geometry_mutex };
			geometry_hierarchies.push_back(hierarchy_buffer);
			return geometry_hierarchies.size() - 1;
		}

		id::id_type create_single_submesh(const void* const data)
		{
			assert(data);
			utl::blob_stream_reader blob{ (const u8*)data };
			// skip lod_count, lod_threshold, submesh_count and size_of_submeshes
			blob.skip(sizeof(u32) + sizeof(f32) + sizeof(u32) + sizeof(u32));
			const u8* at{ blob.position() };
			const id::id_type gpu_id{ graphics::add_submesh(at) };

			static_assert(sizeof(uintptr_t) > sizeof(id::id_type));
			constexpr u8 shift_bits{ (sizeof(uintptr_t) - sizeof(id::id_type)) << 3 };
			u8* const fake_pointer{ (u8* const)((((uintptr_t)gpu_id) << shift_bits) | single_mesh_marker) };
			std::lock_guard lock{ geometry_mutex };
			geometry_hierarchies.push_back(fake_pointer);
			return geometry_hierarchies.size() - 1;
		}

		bool is_signle_mesh(const void* const data)
		{
			assert(data);
			utl::blob_stream_reader blob{ (const u8*)data };
			const u32 lod_count{ blob.read<u32>() };
			assert(lod_count);
			if (lod_count > 1) return false;

			// skip over threshold
			blob.skip(sizeof(f32));
			const u32 submesh_count{ blob.read<u32>() };
			assert(submesh_count);
			return submesh_count == 1;
		}

		constexpr id::id_type gpu_id_from_fake_pointer(u8* const pointer)
		{
			assert((uintptr_t)pointer & single_mesh_marker);
			static_assert(sizeof(uintptr_t) > sizeof(id::id_type));
			constexpr u8 shift_bits{ (sizeof(uintptr_t) - sizeof(id::id_type)) << 3 };
			return (((uintptr_t)pointer) >> shift_bits) & (uintptr_t)id::invalid_id;
		}

		id::id_type create_geometry_resource(const void* const data)
		{
			assert(data);
			return id::invalid_id;
		}

		void destroy_geometry_resource(id::id_type id)
		{
		}

		id::id_type create_material_resource(const void* const data)
		{
			assert(data);
			return graphics::add_material(*(const graphics::material_init_info* const)data);
		}

		void destroy_material_resource(id::id_type id)
		{
			graphics::remove_material(id);
		}
	}

	id::id_type create_resource(const void* const data, asset_type::type type)
	{
		assert(data);
		id::id_type id{ id::invalid_id };

		switch (type)
		{
		case Can::content::asset_type::animation: break;
		case Can::content::asset_type::audio: break;
		case Can::content::asset_type::material: id = create_material_resource(data); break;
		case Can::content::asset_type::mesh: id = create_geometry_resource(data); break;
		case Can::content::asset_type::skeleton: break;
		case Can::content::asset_type::texture: break;
		}

		assert(id::is_valid(id));
		return id;
	}

	void destroy_resource(id::id_type id, asset_type::type type)
	{
		assert(id::is_valid(id));

		switch (type)
		{
		case Can::content::asset_type::animation: break;
		case Can::content::asset_type::audio: break;
		case Can::content::asset_type::material: destroy_material_resource(id); break;
		case Can::content::asset_type::mesh: destroy_geometry_resource(id); break;
		case Can::content::asset_type::skeleton: break;
		case Can::content::asset_type::texture: break;
		default:
			assert(false);
			break;
		}
	}

	id::id_type add_shader(const u8* data)
	{
		const compiled_shader_ptr shader_ptr{ (const compiled_shader_ptr)data };
		const u64 size{ sizeof(u64) + compiled_shader::hash_length + shader_ptr->byte_code_size() };
		std::unique_ptr<u8[]> shader{ std::make_unique<u8[]>(size) };
		memcpy(shader.get(), data, size);
		std::lock_guard lock{ shader_mutex };
		shaders.push_back(std::move(shader));
		return shaders.size() - 1;
	}

	void remove_shader(id::id_type id)
	{
		std::lock_guard lock{ shader_mutex };
		assert(id::is_valid(id));
		shaders.erase(shaders.begin() + id);
	}

	compiled_shader_ptr get_shader(id::id_type id)
	{
		std::lock_guard lock{ shader_mutex };
		assert(id::is_valid(id));
		return (const compiled_shader_ptr)(shaders[id].get());
	}

	void get_submesh_gpu_ids(id::id_type geometry_content_id, u32 id_count, id::id_type* const gpu_ids)
	{
		std::lock_guard lock{ geometry_mutex };
		u8* const pointer{ geometry_hierarchies[geometry_content_id] };
		if ((uintptr_t)pointer & single_mesh_marker)
		{
			assert(id_count == 1);
			*gpu_ids = gpu_id_from_fake_pointer(pointer);
		}
		else
		{
			geometry_hierarchy_stream stream{ pointer };

			assert([&]() {
				const u32 lod_count{ stream.lod_count() };
				const lod_offset lod_offset{ stream.lod_offsets()[lod_count - 1] };
				const u32 gpu_id_count{ (u32)lod_offset.offset + (u32)lod_offset.count };
				return gpu_id_count == id_count;
				}());

			memcpy(gpu_ids, stream.gpu_ids(), sizeof(id::id_type) * id_count);
		}
	}

	void get_lod_offsets(const id::id_type* const geometry_ids, const f32* const thresholds, u32 id_count, std::vector<lod_offset>& offsets)
	{
		assert(geometry_ids && thresholds && id_count);
		assert(offsets.empty());

		std::lock_guard lock{ geometry_mutex };

		for (u32 i{ 0 }; i < id_count; ++i)
		{
			u8* const pointer{ geometry_hierarchies[geometry_ids[i]] };
			if ((uintptr_t)pointer & single_mesh_marker)
			{
				assert(id_count == 1);
				offsets.push_back(lod_offset{ 0, 1 });
			}
			else
			{
				geometry_hierarchy_stream stream{ pointer };
				const u32 lod{ stream.lod_from_threshold(thresholds[i]) };
				offsets.push_back(stream.lod_offsets()[lod]);
			}
		}
	}
}