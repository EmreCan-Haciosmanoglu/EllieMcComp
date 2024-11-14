#pragma once

#include <memory>
#include <typeinfo>

// Platform detection using predefined macros
#ifdef _WIN32
	/* Windows x64/x86 */
#ifndef _WIN64
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

#define BIT(x) (1 << x)

#define CAN_BIND_EVENT_FN(x) std::bind(&Can::x, this, std::placeholders::_1)

namespace Can
{
#include <glm/glm.hpp>
#include <cstdint>
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

	typedef uint8_t u8;
	typedef uint16_t u16;
	typedef uint32_t u32;
	typedef uint64_t u64;

	typedef int8_t s8;
	typedef int16_t s16;
	typedef int32_t s32;
	typedef int64_t s64;

	typedef float f32;
	typedef double f64;

	typedef glm::vec2 v2;
	typedef glm::vec3 v3;
	typedef glm::vec4 v4;
	typedef glm::mat3 m3;
	typedef glm::mat4 m4;

	/*Move these*/
	struct v2i
	{
		s32 x = 0;
		s32 y = 0;
	};
	struct v3i
	{
		v3i(v2i vector, s32 z): x(vector.x), y(vector.y), z(z){}
		v3i(s32 x, s32 y, s32 z): x(x), y(y), z(z){}
		s32 x = 0;
		s32 y = 0;
		s32 z = 0;
	};
}