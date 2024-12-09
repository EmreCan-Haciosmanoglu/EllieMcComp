#pragma once
#ifdef _WIN64
#include "Can/Common/CommonHeaders.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif // !WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace Can::input
{
	HRESULT process_input_message(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
}

#endif // _WIN64