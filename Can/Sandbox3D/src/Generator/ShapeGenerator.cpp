#include "canpch.h"
#include "ShapeGenerator.h"

ShapeGenerator::ShapeGenerator(ShapeSettings* shapeSettings)
	: m_ShapeSettings(shapeSettings)
	, m_NoiseFilter(new NoiseFilter(shapeSettings->GetNoiseSettings()))
{
}

glm::vec3* ShapeGenerator::CalculatePointOnPlanet(const glm::vec3& pointUnitOnSphere)
{
	float elevation = m_NoiseFilter->Evaluate(pointUnitOnSphere);

	glm::vec3* result = new glm::vec3(
		pointUnitOnSphere.x * m_ShapeSettings->GetPlanetRadius() * (1.0f + elevation),
		pointUnitOnSphere.y * m_ShapeSettings->GetPlanetRadius() * (1.0f + elevation),
		pointUnitOnSphere.z * m_ShapeSettings->GetPlanetRadius() * (1.0f + elevation));
	return result;
}
