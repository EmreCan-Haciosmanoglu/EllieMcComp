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
	bool loadOBJ(
		const char* path,
		std::vector < glm::vec3 >& out_vertices,
		std::vector < glm::vec2 >& out_uvs,
		std::vector < glm::vec3 >& out_normals
	);
	void UpdateSphere(int resolution);
	void SetTransform(Can::Object* object, glm::vec3 pos, glm::vec3 scale);
private:
	void ConstructSphere();
	Can::Object* ConstructObject(
		const std::string& shaderPath,
		const std::string& texturePath,
		std::vector < glm::vec3 >& vertices,
		std::vector < glm::vec2 >& uvs,
		std::vector < glm::vec3 >& normals
	);
private:
	int m_Resolution = 4;

	Scene* m_Scene;
	Debug* m_Debug;
	Can::Object* m_Sphere;
	Can::Object* m_TestObject;
};
