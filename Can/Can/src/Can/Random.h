#pragma once
#include "Can.h"

namespace Can::Utility
{
	class Random
	{
	public:
		Random()
		{
			std::srand(std::time(nullptr));
		}
		Random(const Random&) = delete;
		Random& operator=(const Random&) = delete;

		inline static int Integer(int start, int end) { return std::rand() % (end - start) + start; }
		inline static int Integer(int end) { return Integer(0, end); }

		inline static float Float(float start, float end) { return (static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) * (start - end) + start; }
		inline static float Float(float end) { return Float(0, end); }
	};
}