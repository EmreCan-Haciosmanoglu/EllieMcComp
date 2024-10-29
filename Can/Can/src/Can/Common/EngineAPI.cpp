#include "canpch.h"
//#include "Can/Common/CommonHeaders.h"
//#include "Can/Components/Script.h"
//#include "Can/Graphics/Renderer.h"
//#include "Can/Platform/PlatformTypes.h"
//#include "Can/Platform/Platform.h"
//
//#ifndef WIN32_LEAN_AND_MEAN
//#define WIN32_LEAN_AND_MEAN
//#endif // !WIN32_LEAN_AND_MEAN
//
//#include <Windows.h>
//#include <atlsafe.h>
//
//#define EDITOR_INTERFACE 
//using namespace Can;
//
//namespace
//{
//	HMODULE game_code_dll{ nullptr };
//	using _get_script_creator = Can::script::detail::script_creator(*)(size_t);
//	_get_script_creator get_script_creator{ nullptr };
//	using _get_script_names = LPSAFEARRAY(*)(void);
//	_get_script_names get_script_names{ nullptr };
//
//	std::vector<graphics::render_surface> surfaces;
//}
//
//EDITOR_INTERFACE u32 LoadGameCOdeDll(const char* dll_path)
//{
//	if (game_code_dll) return FALSE;
//	game_code_dll = LoadLibraryA(dll_path);
//	assert(game_code_dll);
//
//	get_script_creator = (_get_script_creator)GetProcAddress(game_code_dll, "get_script_creator");
//	get_script_names = (_get_script_names)GetProcAddress(game_code_dll, "get_script_names");
//
//	return (game_code_dll && get_script_creator && get_script_names) ? TRUE : FALSE;
//}
