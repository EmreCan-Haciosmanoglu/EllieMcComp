#pragma once
#include "Can.h"

class NoiseSettings
{
public:
	NoiseSettings();

	inline float GetRoughness() { return roughness; }
	inline float* GetRoughnessPtr() { return &roughness; }

	inline float GetStrength() { return strength; }
	inline float* GetStrengthPtr() { return &strength; }

	inline const glm::vec3& GetCenter() { return center; }
private:
	float strength = 1.0f;
	float roughness = 1.0f;
	glm::vec3 center = { 0, 0, 0 };
};