#pragma once
#include "Can.h"
#include "NoiseSettings.h"

class ShapeSettings
{
public:
	ShapeSettings();

	inline float GetPlanetRadius() { return m_PlanetRadius; }
	inline NoiseSettings* GetNoiseSettings() { return m_NoiseSettings; }
private:
	float m_PlanetRadius = 1.0f;
	NoiseSettings* m_NoiseSettings;
};