#pragma once
#include "Can.h"

class Obstacle
{
public:
	Obstacle(
		float x,
		float h,
		float w,
		float s
	);
public:
	float x;
	float h;
	float w;
	float s;
};