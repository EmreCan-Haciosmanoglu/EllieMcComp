#pragma once
#include "Can.h"
#include "Noise.h"
#include "NoiseSettings.h"

class NoiseFilter
{
public:
	NoiseFilter(NoiseSettings* noiseSettings);

	float Evaluate(const glm::vec3& point);
private:
	Noise* m_Noise;
	NoiseSettings* m_NoiseSettings;
};