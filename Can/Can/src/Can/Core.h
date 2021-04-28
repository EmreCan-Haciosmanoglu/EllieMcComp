#pragma once

#include <memory>

// Platform detection using predefined macros
#ifdef _WIN32
	/* Windows x64/x86 */
#ifdef _WIN64
	/* Windows x64  */
	#define CAN_PLATFORM_WINDOWS
#else
	#error "x86 Builds are not supported!"
#endif
#elif defined(__APPLE__) || defined(__MACH__)
	#include <TargetConditionals.h>
#if TARGET_IPHONE_SIMULATOR == 1
	#error "IOS simulator is not supported!"
#elif TARGET_OS_IPHONE == 1
	#define CAN_PLATFORM_IOS
#error "IOS is not supported!"
#elif TARGET_OS_MAC == 1
	#define CAN_PLATFORM_MACOS
	#error "MacOS is not supported!"
#else
	#error "Unknown Apple platform!"
#endif
#elif defined(__ANDROID__)
	#define CAN_PLATFORM_ANDROID
	#error "Android is not supported!"
#elif defined(__linux__)
	#define CAN_PLATFORM_LINUX
	#error "Linux is not supported!"
#else
	#error "Unknown platform!"
#endif 

#ifdef CAN_DEBUG
#define CAN_ENABLE_ASSERTS
#endif

#ifdef CAN_ENABLE_ASSERTS
#define CAN_ASSERT(x, ...) { if(!(x)) { CAN_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#define CAN_CORE_ASSERT(x, ...) { if(!(x)) { CAN_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
#define CAN_ASSERT(x, ...)
#define CAN_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define CAN_BIND_EVENT_FN(x) std::bind(&Can::x, this, std::placeholders::_1)

namespace Can
{
#include <glm/glm.hpp>
	typedef void (*OnclickCallbackFn)();

	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	typedef unsigned char u8;
	typedef unsigned short u16;
	typedef unsigned int  u32;
	typedef unsigned long long u64;

	typedef char s8;
	typedef short s16;
	typedef int  s32;
	typedef long long s64;

	typedef float f32;
	typedef double f64;

	typedef glm::vec2 v2;
	typedef glm::vec3 v3;
	typedef glm::vec4 v4;
	typedef glm::mat3 m3;
	typedef glm::mat4 m4;
}