#pragma once

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>
#include <limits>

#include <string>
#include <ctime>
#include <cstdlib>
#include <sstream>
#include <fstream>
#include <vector>
#include <array>
#include <unordered_map>
#include <unordered_set>
#include <filesystem>
#include <glm/glm.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/vector_angle.hpp> 

#include "../stb/stb_image.h"

#include "Can/Log.h"
#include "Can/KeyCode.h"
#include "Can/MouseButtonCode.h"

#ifdef CAN_PLATFORM_WINDOWS
	#include <Windows.h>
#endif

#include "Can/Instrumentor.h"