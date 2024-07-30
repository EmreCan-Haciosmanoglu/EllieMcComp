#pragma once
#include "Can.h"

namespace Can
{
	class Random
	{
	public:
		Random();
		~Random() = default;
		Random(const Random&) = delete;
		Random& operator=(const Random&) = delete;
	};

	inline static u8 random_u8(u8 start, u8 end) { return rand() % (end - start) + start; }
	inline static u8 random_u8(u8 end) { return random_u8(0, end); }

	inline static u16 random_u16(u16 start, u16 end) { return rand() % (end - start) + start; }
	inline static u16 random_u16(u16 end) { return random_u16(0, end); }

	inline static u64 random_u64(u64 start, u64 end) { return rand() % (end - start) + start; }
	inline static u64 random_u64(u64 end) { return random_u64(0, end); }

	inline static int random_s32(s32 start, s32 end) { return rand() % (end - start) + start; }
	inline static int random_s32(s32 end) { return random_s32(0, end); }

	inline static f32 random_f32(f32 start, f32 end) { return (static_cast <f32> (rand()) / static_cast <f32> (RAND_MAX)) * (end - start) + start; }
	inline static f32 random_f32(f32 end) { return random_f32(0.0f, end); }

	inline static f64 random_f64(f64 start, f64 end) { return (static_cast <f64> (rand()) / static_cast <f64> (RAND_MAX)) * (end - start) + start; }
	inline static f64 random_f64(f64 end) { return random_f64(0.0f, end); }
}