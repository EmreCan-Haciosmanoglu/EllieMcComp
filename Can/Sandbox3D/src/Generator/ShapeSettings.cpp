#include "canpch.h"
#include "ShapeSettings.h"

ShapeSettings::ShapeSettings(int layerCount)
	: m_LayerCount(layerCount)
	, m_NoiseLayers(new NoiseLayer*[m_LayerCount])
{
	for (int i = 0; i < m_LayerCount; i++)
		m_NoiseLayers[i] = new NoiseLayer(new NoiseSettings());
}
