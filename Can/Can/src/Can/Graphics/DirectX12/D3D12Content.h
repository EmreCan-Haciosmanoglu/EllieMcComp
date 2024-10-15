#pragma once
#include "D3D12CommonHeaders.h"
#include "D3D12Helpers.h"
#include "Can\Common\Id.h"
#include "Can\Core.h"

#include "Can\Utilities\IOStream.h"

namespace Can::graphics::d3d12::content
{
	namespace submesh
	{
		id::id_type add(const u8*& data);
		void remove(id::id_type id);
	}
}