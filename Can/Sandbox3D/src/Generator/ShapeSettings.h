#pragma once
class ShapeSettings
{
public:
	ShapeSettings();

	inline float GetPlanetRadius() { return m_PlanetRadius; }
private:
	float m_PlanetRadius = 1.0f;
};