#pragma once
#include "Can.h"
#include "NoiseLayer.h"
class ShapeSettings
{
public:
	ShapeSettings(int layerCount);

	inline int GetLayerCount() { return m_LayerCount; }
	inline float GetPlanetRadius() { return m_PlanetRadius; }
	inline NoiseLayer** GetNoiseLayers() { return m_NoiseLayers; }
private:
	int m_LayerCount;
	float m_PlanetRadius = 5.0f;
	NoiseLayer** m_NoiseLayers;
};