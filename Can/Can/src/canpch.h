#pragma once

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "Can/Log.h"
#include "Can/KeyCode.h"
#include "Can/MouseButtonCode.h"

#ifdef CAN_PLATFORM_WINDOWS
	#include <Windows.h>
#endif