#include "canpch.h"
#include "ContentLoader.h"

#include "Can\Components\Entity.h"
#include "Can\Components\Transform.h"
#include "Can\Components\Script.h"

#include "Can\Graphics\Renderer.h"

#if !defined(SHIPPING)

#include <fstream>
#include <filesystem>
#include <Windows.h>

namespace Can::content
{
	namespace
	{
		enum component_type
		{
			transform = 0,
			script,

			count
		};

		std::vector<game_entity::entity> entities;
		transform::init_info transform_info{};
		script::init_info script_info{};

		bool read_transform(const u8*& data, game_entity::entity_info& info)
		{
			return true;
		}

		bool read_script(const u8*& data, game_entity::entity_info& info)
		{
			return true;
		}

		using component_reader = bool(*)(const u8*&, game_entity::entity_info&);
		component_reader component_readers[]
		{
			read_transform,
			read_script
		};
		static_assert(_countof(component_readers) == component_type::count);

		bool read_file(std::filesystem::path path, std::unique_ptr<u8[]>& data, u64& size)
		{
			if (!std::filesystem::exists(path)) return false;

			size = std::filesystem::file_size(path);
			assert(size);
			if (!size) return false;
			data = std::make_unique < u8[]>(size);
			std::ifstream file{ path, std::ios::in | std::ios::binary };
			if (!file || !file.read((char*)data.get(), size))
			{
				file.close();
				return false;
			}
			file.close();
			return true;
		}
	}
	bool load_game()
	{
		std::unique_ptr<u8[]> game_data{};
		u64 size{ 0 };
		if (!read_file("game.bin", game_data, size)) return false;
		assert(game_data.get());
		const u8* at{ game_data.get() };
		constexpr u32 su32{ sizeof(u32) };
		const u32 num_entities{ *at }; at += su32;
		if (!num_entities) return false;

		for (u32 entity_index{ 0 }; entity_index < num_entities; ++entity_index)
		{
			game_entity::entity_info info{};
			const u32 entity_type{ *at }; at += su32;
			const u32 num_components{ *at }; at += su32;
			if (!num_components) return false;
			for (u32 component_index{ 0 }; component_index < num_components; ++component_index)
			{
				const u32 component_type{ *at }; at += su32;
				assert(component_type < component_type::count);
				if (!component_readers[component_type](at, info)) return false;
			}

			assert(info.transform);
			game_entity::entity entity{ game_entity::create(info) };
			if (!entity.is_valid()) return false;
			entities.emplace_back(entity);
		}

		assert(at == game_data.get() + size);
		return true;
	}

	void unload_game()
	{
		for (auto entity : entities)
		{
			game_entity::remove(entity.get_id());
		}
	}

	bool load_engine_shaders(std::unique_ptr<u8[]>& shaders, u64 size)
	{
		auto path = graphics::get_engine_shaders_path();
		return read_file(path, shaders, size);
	}
}
#endif