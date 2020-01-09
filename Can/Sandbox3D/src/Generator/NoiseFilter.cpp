#include "canpch.h"
#include "NoiseFilter.h"

NoiseFilter::NoiseFilter()
	: m_Noise(new Noise())
{
}

float NoiseFilter::Evaluate(const glm::vec3& point)
{
	float noiseValue = (m_Noise->Evaluate(point) + 1.0f) * 0.5f;
	return noiseValue;
}
