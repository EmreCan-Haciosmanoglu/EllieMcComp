#pragma once
#include "Can.h"
#include "Noise.h"

class NoiseFilter
{
public:
	NoiseFilter();

	float Evaluate(const glm::vec3& point);
private:
	Noise* m_Noise;
};