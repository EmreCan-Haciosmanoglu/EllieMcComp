#include "canpch.h"
#include "Random.h"

namespace Can::Utility
{
	Random::Random()
	{
		std::srand((unsigned int)std::time(nullptr));
	}
}