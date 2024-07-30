#include "canpch.h"
#include "Random.h"

namespace Can
{
	Random::Random()
	{
		std::srand((unsigned int)std::time(nullptr));
	}
}