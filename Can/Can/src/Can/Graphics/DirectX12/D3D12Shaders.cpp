#include "canpch.h"
#include "D3D12Shaders.h"
#include "Can\Content\ContentLoader.h"
#include "Can\Content\ContentToEngine.h"

namespace Can::graphics::d3d12::shaders
{
	namespace
	{
		content::compiled_shader_ptr engine_shaders[engine_shader::count]{};

		std::unique_ptr<u8[]> engine_shaders_blob{};
		bool load_engine_shaders()
		{
			assert(!engine_shaders_blob);
			u64 size{ 0 };
			bool result{ content::load_engine_shaders(engine_shaders_blob, size) };
			assert(engine_shaders_blob && size);

			u64 offset{ 0 };
			u32 index{ 0 };
			while (offset < size && result)
			{
				assert(index < engine_shader::count);
				content::compiled_shader_ptr& shader{ engine_shaders[index] };
				assert(!shader);
				result &= index < engine_shader::count && !shader;
				if (!result) break;
				shader = reinterpret_cast<const content::compiled_shader_ptr>(&engine_shaders_blob[offset]);
				offset += shader->buffer_size();
				++index;
			}
			assert(offset == size && index == engine_shader::count);

			return true;
		}
	}

	bool initialize()
	{
		return load_engine_shaders();
	}

	void shutdown()
	{
		for (u32 i{ 0 }; i < engine_shader::count; ++i) engine_shaders[i] = {};
		engine_shaders_blob.reset();
	}

	D3D12_SHADER_BYTECODE get_engine_shader(engine_shader::id id)
	{
		assert(id < engine_shader::count);
		const content::compiled_shader_ptr shader{ engine_shaders[id] };
		assert(shader && shader->byte_code_size());

		return { shader->byte_code(), shader->byte_code_size() };
	}
}