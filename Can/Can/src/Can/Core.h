#pragma once

#ifdef CAN_PLATFORM_WINDOWS
	#ifdef CAN_BUILD_DLL
		#define CAN_API __declspec(dllexport)
	#else
		#define CAN_API __declspec(dllimport)
	#endif // CAN_BUILD_DLL
#else
	#error CAN only supports Windows!
#endif // CAN_PLATFORM_WINDOWS