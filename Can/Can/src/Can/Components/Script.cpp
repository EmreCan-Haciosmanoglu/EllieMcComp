#include "canpch.h"
#include "Script.h"
#include "Transform.h"
#include "Can/Unordered_Array.h"

namespace Can::script
{
	namespace
	{
		utl::vector<detail::script_ptr>         entity_scripts;
		utl::vector<id::id_type>                id_mapping;

		utl::vector<id::generation_type>        generations;
		utl::deque<script_id>                   free_ids;

		utl::vector<transform::component_cache> transform_cache;
		std::unordered_map<id::id_type, u32>    cache_map;

		using script_registery = std::unordered_map < size_t, detail::script_creator>;

		script_registery& registery()
		{
			static	script_registery reg;
			return reg;
		}
		bool exists(script_id id)
		{
			assert(id::is_valid(id));
			const id::id_type index{ id::index(id) };
			assert(index < generations.size() && id_mapping[index] < entity_scripts.size());
			assert(generations[index] == id::generation(id));
			return generations[index] == id::generation(id) &&
				entity_scripts[id_mapping[index]] &&
				entity_scripts[id_mapping[index]]->is_valid();
		}

		transform::component_cache* const get_cache_ptr(const game_entity::entity* const entity)
		{
			assert(game_entity::is_alive(entity->get_id()));
			const transform::transform_id id{ entity->transform().get_id() };

			u32 index{ u32_invalid_id };

			auto pair = cache_map.try_emplace(id, id::invalid_id);

			if (pair.second)
			{
				index = (u32)transform_cache.size();
				transform_cache.emplace_back();
				transform_cache.back().id = id;
				cache_map[id] = index;
			}
			else
			{
				index = cache_map[id];
			}

			assert(index < transform_cache.size());
			return &transform_cache[index];
		}
	}

	namespace detail
	{
		u8 register_script(size_t tag, script_creator func)
		{
			bool result{ registery().insert(script_registery::value_type{tag,func}).second };
			assert(result);
			return result;
		}

		script_creator get_script_creator(size_t tag)
		{
			auto script = Can::script::registery().find(tag);
			assert(script != Can::script::registery().end() && script->first == tag);
			return script->second;
		}
	}

	component create(init_info info, game_entity::entity entity)
	{
		assert(entity.is_valid());
		assert(info.script_creator);

		script_id id{};
		if (free_ids.size() > id::min_deleted_elements)
		{
			id = free_ids.front();
			assert(!exists(id));
			free_ids.pop_front();
			id = script_id{ id::new_generation(id) };
			++generations[id::index(id)];
		}
		else
		{
			id == script_id{ (id::id_type)id_mapping.size() };
			id_mapping.emplace_back();
			generations.push_back(0);
		}

		assert(id::is_valid(id));
		const id::id_type index{ (id::id_type)entity_scripts.size() };
		entity_scripts.emplace_back(info.script_creator(entity));
		assert(entity_scripts.back()->get_id() == entity.get_id());
		id_mapping[id::index(id)] = index;
		return component{ id };
	}

	void remove([[maybe_unused]] component c)
	{
		assert(c.is_valid() && exists(c.get_id()));
		const script_id id{ c.get_id() };
		const id::id_type index{ id_mapping[id::index(id)] };
		const script_id last_id{ entity_scripts.back()->script().get_id() };
		utl::erase_unordered(entity_scripts, index);
		id_mapping[id::index(last_id)] = index;
		id_mapping[id::index(id)] = id::invalid_id;
	}

	void update(float dt)
	{
		for (auto& ptr : entity_scripts)
		{
			ptr->update(dt);
		}

		if (transform_cache.size())
		{
			transform::update(transform_cache.data(), (u32)transform_cache.size());
			transform_cache.clear();

			cache_map.clear();
		}
	}

	void entity_script::set_rotation(const game_entity::entity* const entity, math::v4 rotation_quaternion)
	{
		transform::component_cache& cache{ *get_cache_ptr(entity) };
		cache.flags |= transform::component_flags::rotation;
		cache.rotation = rotation_quaternion;
	}

	void entity_script::set_orientation(const game_entity::entity* const entity, math::v3 orientation_vector)
	{
		transform::component_cache& cache{ *get_cache_ptr(entity) };
		cache.flags |= transform::component_flags::orientation;
		cache.orientation = orientation_vector;
	}

	void entity_script::set_position(const game_entity::entity* const entity, math::v3 position)
	{
		transform::component_cache& cache{ *get_cache_ptr(entity) };
		cache.flags |= transform::component_flags::position;
		cache.position = position;
	}

	void entity_script::set_scale(const game_entity::entity* const entity, math::v3 scale)
	{
		transform::component_cache& cache{ *get_cache_ptr(entity) };
		cache.flags |= transform::component_flags::scale;
		cache.scale = scale;
	}
}