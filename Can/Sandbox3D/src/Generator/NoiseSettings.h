#pragma once
#include "Can.h"

class NoiseSettings
{
public:
	NoiseSettings();

	inline float GetMinValue() { return minValue; }
	inline float* GetMinValuePtr() { return &minValue; }

	inline float GetBaseRoughness() { return baseRoughness; }
	inline float* GetBaseRoughnessPtr() { return &baseRoughness; }

	inline int GetNumLayer() { return numLayer; }
	inline int* GetNumLayerPtr() { return &numLayer; }

	inline float GetPersistence() { return persistence; }
	inline float* GetPersistencePtr() { return &persistence; }

	inline float GetRoughness() { return roughness; }
	inline float* GetRoughnessPtr() { return &roughness; }

	inline float GetStrength() { return strength; }
	inline float* GetStrengthPtr() { return &strength; }

	inline const glm::vec3& GetCenter() { return center; }
	inline glm::vec3* GetCenterPtr() { return &center; }
private:
	int numLayer = 2;
	float strength = 1.0f;
	float baseRoughness = 1.0f;
	float roughness = 2.0f;
	float persistence = 0.5f;
	float minValue = 0.0f;
	glm::vec3 center = { 0, 0, 0 };
};