#include "canpch.h"
#include "Scene.h"

namespace Can
{
	Scene::Scene()
		: m_Registry()
		, entityID(m_Registry.create())
	{
	}

	Scene::~Scene()
	{
	}
}