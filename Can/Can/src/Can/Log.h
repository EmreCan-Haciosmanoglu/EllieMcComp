#pragma once

#include "Core.h"

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

namespace Can
{
	class Log
	{
	public:
		static void Init();

		inline static Ref<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static Ref<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static Ref<spdlog::logger> s_CoreLogger;
		static Ref<spdlog::logger> s_ClientLogger;
	};
}

//Core log macros
#define CAN_CORE_TRACE(...) ::Can::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define CAN_CORE_INFO(...)  ::Can::Log::GetCoreLogger()->info(__VA_ARGS__)
#define CAN_CORE_WARN(...)  ::Can::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define CAN_CORE_ERROR(...) ::Can::Log::GetCoreLogger()->error(__VA_ARGS__)
#define CAN_CORE_CRITICAL(...) ::Can::Log::GetCoreLogger()->critical(__VA_ARGS__)

//Client log macros
#define CAN_TRACE(...)      ::Can::Log::GetClientLogger()->trace(__VA_ARGS__)
#define CAN_INFO(...)       ::Can::Log::GetClientLogger()->info(__VA_ARGS__)
#define CAN_WARN(...)       ::Can::Log::GetClientLogger()->warn(__VA_ARGS__)
#define CAN_ERROR(...)      ::Can::Log::GetClientLogger()->error(__VA_ARGS__)
#define CAN_CRITICAL(...)      ::Can::Log::GetClientLogger()->critical(__VA_ARGS__)