#pragma once
#include "Can.h"

namespace Can::Utility
{
	class Random
	{
	public:
		Random();
		~Random() = default;
		Random(const Random&) = delete;
		Random& operator=(const Random&) = delete;

		inline static u8 unsigned_8(u8 start, u8 end) { return rand() % (end - start) + start; }
		inline static u8 unsigned_8(u8 end) { return unsigned_8(0, end); }

		inline static u16 unsigned_16(u16 start, u16 end) { return rand() % (end - start) + start; }
		inline static u16 unsigned_16(u16 end) { return unsigned_16(0, end); }

		inline static u64 unsigned_64(u64 start, u64 end) { return rand() % (end - start) + start; }
		inline static u64 unsigned_64(u64 end) { return unsigned_64(0, end); }

		inline static int signed_32(s32 start, s32 end) { return rand() % (end - start) + start; }
		inline static int signed_32(s32 end) { return signed_32(0, end); }

		inline static float Float(float start, float end) { return (static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) * (end - start) + start; }
		inline static float Float(float end) { return Float(0.0f, end); }
	};
}