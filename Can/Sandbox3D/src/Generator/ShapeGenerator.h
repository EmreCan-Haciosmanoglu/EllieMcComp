#pragma once
#include "Can.h"
#include "NoiseFilter.h"
#include "ShapeSettings.h"

class ShapeGenerator
{
public:
	ShapeGenerator(ShapeSettings* shapeSettings);
	glm::vec3 CalculatePointOnPlanet(const glm::vec3& pointOnSphere);

	inline ShapeSettings* GetShapeSettings() { return m_ShapeSettings; }
private:
	int m_FilterCount;

	ShapeSettings* m_ShapeSettings;
	NoiseFilter** m_NoiseFilters;
};