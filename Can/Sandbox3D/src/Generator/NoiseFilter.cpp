#include "canpch.h"
#include "NoiseFilter.h"

NoiseFilter::NoiseFilter(NoiseSettings* noiseSettings)
	: m_Noise(new Noise())
	, m_NoiseSettings(noiseSettings)
{
}

float NoiseFilter::Evaluate(const glm::vec3& point)
{
	glm::vec3 p(
		point.x * m_NoiseSettings->GetRoughness() + m_NoiseSettings->GetCenter().x,
		point.y * m_NoiseSettings->GetRoughness() + m_NoiseSettings->GetCenter().y,
		point.z * m_NoiseSettings->GetRoughness() + m_NoiseSettings->GetCenter().z
	);
	
	float noiseValue = (m_Noise->Evaluate(p) + 1.0f) * 0.5f;
	return noiseValue * m_NoiseSettings->GetStrength();
}
