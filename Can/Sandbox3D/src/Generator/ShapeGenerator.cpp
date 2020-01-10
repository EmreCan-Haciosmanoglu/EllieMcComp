#include "canpch.h"
#include "ShapeGenerator.h"

ShapeGenerator::ShapeGenerator(ShapeSettings* shapeSettings)
	: m_FilterCount(shapeSettings->GetLayerCount())
	, m_ShapeSettings(shapeSettings)
	, m_NoiseFilters(new NoiseFilter* [m_FilterCount])
{
	NoiseLayer** layers = m_ShapeSettings->GetNoiseLayers();

	for (int i = 0; i < m_FilterCount; i++)
		m_NoiseFilters[i] = new NoiseFilter(layers[i]->GetNoiseSettings());
}

glm::vec3 ShapeGenerator::CalculatePointOnPlanet(const glm::vec3 & pointUnitOnSphere)
{
	float maskVal = 0;
	float elevation = 0.0f;

	if (m_FilterCount > 0)
	{
		maskVal = m_NoiseFilters[0]->Evaluate(pointUnitOnSphere);

		if (m_ShapeSettings->GetNoiseLayers()[0]->IsEnabled())
			elevation = maskVal;
	}

	for (int i = 1; i < m_FilterCount; i++)
		if (m_ShapeSettings->GetNoiseLayers()[i]->IsEnabled())
			elevation += m_NoiseFilters[i]->Evaluate(pointUnitOnSphere) * 
			(m_ShapeSettings->GetNoiseLayers()[0]->IsMask() ? maskVal : 1);

	return pointUnitOnSphere * m_ShapeSettings->GetPlanetRadius() * (1.0f + elevation);
}
