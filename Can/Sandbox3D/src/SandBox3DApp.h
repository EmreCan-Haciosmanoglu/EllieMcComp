#pragma once
#include "Can.h"

#include "Scene.h"
#include "Debug.h"
#include "Generator/ShapeGenerator.h"

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
	void UpdateSphere();
	void UpdateSphereFace(Can::Object* m_SphereFace, const glm::vec3& localUp, float xOffSet, float yOffSet);
	void SetTransform(Can::Object* object, glm::vec3 pos, glm::vec3 scale);
	
	inline ShapeGenerator* GetShapeGenerator() { return m_ShapeGenerator; }
	inline int GetFaceCount() { return 6 * m_FaceResolution * m_FaceResolution; }
	inline Can::Object** GetFaces() { return m_SphereFaces; }

private:
	float* CreateSphere(const glm::vec3& localUp, float xOffSet, float yOffSet);
	float* ShapeSphere(float* cube);
	float* FillNormals(float* sphere);
	uint32_t* FillIndices();
	void ConstructSphereFace(Can::Object* m_SphereFace, float xOffSet, float yOffSet, const glm::vec3& localUp);
	Can::Object* ConstructObject(
		const std::string& shaderPath,
		const std::string& texturePath,
		std::vector < glm::vec3 >& vertices,
		std::vector < glm::vec2 >& uvs,
		std::vector < glm::vec3 >& normals
	);

private:
	int m_Resolution = 5;
	int m_FaceResolution = 1;

	glm::vec3 m_LocalUps[6] = {
		{+1.0f, +0.0f, +0.0f},
		{-1.0f, +0.0f, +0.0f},
		{+0.0f, +1.0f, +0.0f},
		{+0.0f, -1.0f, +0.0f},
		{+0.0f, +0.0f, +1.0f},
		{+0.0f, +0.0f, -1.0f}
	};

	ShapeGenerator* m_ShapeGenerator;
	Scene* m_Scene;
	Debug* m_Debug;
	Can::Object** m_SphereFaces;
	Can::Object* m_TestObject = nullptr;
};
