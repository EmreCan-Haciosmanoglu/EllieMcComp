#pragma once
#include "Can.h"
#include "NoiseSettings.h"

class NoiseLayer
{
public:
	NoiseLayer(NoiseSettings* noiseSettings);

	inline NoiseSettings* GetNoiseSettings() { return m_NoiseSettings; }

	inline void Enable(bool status) { m_Enabled = true; }

	inline bool IsEnabled() { return m_Enabled; }
	inline bool* GetEnabledPtr() { return &m_Enabled; }

	inline bool IsMask() { return m_IsMask; }
	inline bool* GetMaskPtr() { return &m_IsMask; }

private:
	bool m_Enabled = true;
	bool m_IsMask = true;
	NoiseSettings* m_NoiseSettings;
};