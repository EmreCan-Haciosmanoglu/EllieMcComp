#pragma once

#include <memory>

#ifdef CAN_PLATFORM_WINDOWS
#ifdef CAN_DYNAMIC_LINK
#ifdef CAN_BUILD_DLL
#define CAN_API __declspec(dllexport)
#else
#define CAN_API __declspec(dllimport)
#endif // CAN_BUILD_DLL
#else
#define CAN_API 
#endif
#else
#error CAN only supports Windows!
#endif // CAN_PLATFORM_WINDOWS

#ifdef CAN_DEBUG
#define CAN_ENABLE_ASSERTS
#endif  // CAN_DEBUG

#ifdef CAN_ENABLE_ASSERTS
#define CAN_ASSERT(x, ...) {if(!(x)) { CAN_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak();}}
#define CAN_CORE_ASSERT(x, ...) {if(!(x)) { CAN_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak();}}
#else
#define CAN_ASSERT(x, ...)
#define CAN_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define CAN_BIND_EVENT_FN(x) std::bind(&Can::x,this,std::placeholders::_1)

namespace Can
{
	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T>
	using Ref = std::shared_ptr<T>;
}