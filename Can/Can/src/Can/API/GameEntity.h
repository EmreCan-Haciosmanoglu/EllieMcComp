#pragma once

#include "Can/Components/ComponentsCommonHeaders.h"
#include "TransformComponent.h"
#include "ScriptComponent.h"

#include <memory>
#include <bitset>

namespace Can
{
	namespace game_entity
	{
		DEFINE_TYPED_ID(entity_id);

		class entity
		{
		public:
			constexpr explicit entity(entity_id id) : _id{ id } {}
			constexpr entity() : _id{ id::invalid_id } {}
			constexpr entity_id get_id() const { return _id; }
			constexpr bool is_valid() const { return id::is_valid(_id); }

			transform::component transform() const;
			script::component script() const;
		private:
			entity_id _id;
		};
	}
	namespace script
	{
		class entity_script : public game_entity::entity
		{
		public:
			virtual ~entity_script() = default;
			virtual void begin_play() {}
			virtual void update(f32) {}
		protected:
			constexpr explicit entity_script(game_entity::entity entity)
				:game_entity::entity{ entity.get_id() }{}
		};

		namespace detail
		{
			using script_ptr = std::unique_ptr<script::entity_script>;
			using script_creator = script_ptr(*)(game_entity::entity entity);
			using script_hash = std::hash<std::string>;

			u8 register_script(size_t, script_creator);

			template<class script_class>
			script_ptr create_script(game_entity::entity entity)
			{
				assert(entity.is_valid());
				return std::make_unique< script_class>(entity);
			}

#define REGISTER_SCRIPT(TYPE)												\
			class TYPE;														\
			namespace														\
			{																\
				const u8 _reg##TYPE											\
				{															\
					Can::script::detail::register_script(					\
						Can::script::detail::script_hash()(#TYPE),			\
						&Can::script::detail::create_script<TYPE>			\
					)														\
				};															\
			}
		}
	}
}