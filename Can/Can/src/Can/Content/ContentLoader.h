#pragma once
#include "Can/Graphics/DirectX12/D3D12CommonHeaders.h"

#if !defined(SHIPPING)
namespace Can::content
{
	bool load_game();
	void unload_game();

	bool load_engine_shaders(std::unique_ptr<u8[]>& shaders, u64& size);
}
#endif // !defined(SHIPPING)