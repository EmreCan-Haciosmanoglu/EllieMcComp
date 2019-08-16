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

#ifdef CAN_ENABLE_ASSERTS
#define CAN_ASSERT(x, ...) {if(!(x)) { CAN_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak();}}
#define CAN_CORE_ASSERT(x, ...) {if(!(x)) { CAN_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak();}}
#else
#define CAN_ASSERT(x, ...)
#define CAN_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)
