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

		entt::entity entityID;
		entt::registry m_Registry;
	};
}