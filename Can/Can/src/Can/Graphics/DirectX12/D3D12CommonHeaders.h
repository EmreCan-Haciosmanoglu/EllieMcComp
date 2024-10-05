#pragma once
#include "Can\Common\CommonHeaders.h"
#include "Can\Graphics\Renderer.h"
#include "Can\Platform\Window.h"

#include <dxgi1_6.h>
#include <d3d12.h>
#include <wrl.h>

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d12.lib")

namespace Can::graphics::d3d12
{
	constexpr u32 frame_buffer_count{ 3 };
}

#ifdef _DEBUG
#ifndef DXCall
#define DXCall(x)								\
if (FAILED(x)){									\
	char line_number[32];						\
	sprintf_s(line_number, "%u", __LINE__);		\
	OutputDebugStringA("Error in:");			\
	OutputDebugStringA(__FILE__);				\
	OutputDebugStringA("\nLine:");				\
	OutputDebugStringA(line_number);			\
	OutputDebugStringA("\n");					\
	OutputDebugStringA(#x);						\
	OutputDebugStringA("\n");					\
	__debugbreak();								\
}
#endif // !DXCall
#else
#ifndef DXCall
#define DXCall(x) x
#endif // !DXCall
#endif // _DEBUG

#ifdef _DEBUG
#ifndef NAME_D3D12_OBJECT
#define NAME_D3D12_OBJECT(obj, name)				\
obj->SetName(name);									\
OutputDebugString(L"::D3D12 Object Created: ");		\
OutputDebugString(name);							\
OutputDebugString(L"\n");							
#endif // !NAME_D3D12_OBJECT
#else
#ifndef NAME_D3D12_OBJECT
#define NAME_D3D12_OBJECT(obj, name)
#endif // !NAME_D3D12_OBJECT
#endif // _DEBUG

#ifdef _DEBUG
#ifndef NAME_D3D12_OBJECT_INDEXED
#define NAME_D3D12_OBJECT_INDEXED(obj, index, name)				\
{														\
wchar_t full_name[128];									\
if(swprintf_s(full_name, L"%s[%u]", name, index) > 0){	\
obj->SetName(full_name);								\
OutputDebugString(L"::D3D12 Object Created: ");			\
OutputDebugString(full_name);							\
OutputDebugString(L"\n");								\
}}
#endif // !NAME_D3D12_OBJECT_INDEXED
#else
#ifndef NAME_D3D12_OBJECT_INDEXED
#define NAME_D3D12_OBJECT_INDEXED(obj, index, name)
#endif // !NAME_D3D12_OBJECT_INDEXED
#endif // _DEBUG