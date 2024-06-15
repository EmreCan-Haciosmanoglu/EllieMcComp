#pragma once

#ifndef _SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING
#define _SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING
#endif // !_SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING


#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>
#include <limits>
#include <format>

#include <string>
#include <ctime>
#include <cstdlib>
#include <sstream>
#include <fstream>
#include <vector>
#include <array>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <filesystem>
#include <glm/glm.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "../stb/stb_image.h"

#include "Can/Log.h"
#include "Can/KeyCodes.h"
#include "Can/MouseCodes.h"

#ifdef CAN_PLATFORM_WINDOWS
	#include <Windows.h>
#endif

#include "Can/Instrumentor.h"