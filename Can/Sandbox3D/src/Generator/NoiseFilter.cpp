#include "canpch.h"
#include "NoiseFilter.h"

NoiseFilter::NoiseFilter(NoiseSettings* noiseSettings)
	: m_Noise(new Noise())
	, m_NoiseSettings(noiseSettings)
{
}

float NoiseFilter::Evaluate(const glm::vec3& point)
{
	float noiseValue = 0.0f;
	float frequency = m_NoiseSettings->GetBaseRoughness();
	float amplitude = 1.0f;

	for (int i = 0; i < m_NoiseSettings->GetNumLayer(); i++)
	{
		float v = m_Noise->Evaluate(point * frequency + m_NoiseSettings->GetCenter());
		noiseValue += (v + 1.0f) * 0.5f * amplitude;
		frequency *= m_NoiseSettings->GetRoughness();
		amplitude *= m_NoiseSettings->GetPersistence();
	}

	noiseValue = std::max(0.0f, noiseValue - m_NoiseSettings->GetMinValue());

	return noiseValue * m_NoiseSettings->GetStrength();
}
