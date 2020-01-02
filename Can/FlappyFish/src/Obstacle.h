#pragma once
#include "Can.h"

class Obstacle
{
public:
	Obstacle(
		float x1,
		float x2,
		float y1,
		float y2,
		float h1,
		float h2,
		float w,
		float s
	);
public:
	float x1, x2;
	float y1, y2;
	float h1, h2;
	float w;
	float s;
};