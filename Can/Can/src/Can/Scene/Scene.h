#pragma once
#include "Can.h"
#include <entt/entt.hpp>

namespace Can
{
	class Scene
	{
	public:
		Scene();
		~Scene();

		entt::registry m_Registry;
		entt::entity entityID;
	};
}