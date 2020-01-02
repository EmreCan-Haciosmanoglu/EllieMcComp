#pragma once
#include "Can.h"

#include "Scene.h"
#include "Debug.h"

class Sandbox3D : public Can::Application
{
public:
	Sandbox3D();
	~Sandbox3D();

	inline int* GetResolutionPtr() { return &m_Resolution; }

private:
	int m_Resolution = 4;

	Scene* m_Scene;
	Debug* m_Debug;
};