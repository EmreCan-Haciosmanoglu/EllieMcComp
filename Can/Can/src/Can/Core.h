#pragma once

#include <memory>

// Platform detection using predefined macros
#ifdef _WIN32
	/* Windows x64/x86 */
#ifdef _WIN64
	/* Windows x64  */
#define HZ_PLATFORM_WINDOWS
#else
	/* Windows x86 */
#error "x86 Builds are not supported!"
#endif
#elif defined(__APPLE__) || defined(__MACH__)
#include <TargetConditionals.h>
/* TARGET_OS_MAC exists on all the platforms
 * so we must check all of them (in this order)
 * to ensure that we're running on MAC
 * and not some other Apple platform */
#if TARGET_IPHONE_SIMULATOR == 1
#error "IOS simulator is not supported!"
#elif TARGET_OS_IPHONE == 1
#define HZ_PLATFORM_IOS
#error "IOS is not supported!"
#elif TARGET_OS_MAC == 1
#define HZ_PLATFORM_MACOS
#error "MacOS is not supported!"
#else
#error "Unknown Apple platform!"
#endif
 /* We also have to check __ANDROID__ before __linux__
  * since android is based on the linux kernel
  * it has __linux__ defined */
#elif defined(__ANDROID__)
#define HZ_PLATFORM_ANDROID
#error "Android is not supported!"
#elif defined(__linux__)
#define HZ_PLATFORM_LINUX
#error "Linux is not supported!"
#else
	/* Unknown compiler/platform */
#error "Unknown platform!"
#endif // End of platform detection

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