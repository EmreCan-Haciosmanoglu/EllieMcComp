#pragma once
#include "D3D12CommonHeaders.h"
#include "D3D12Helpers.h"
#include "Can\Core.h"

#include "Can\Utilities\IOStream.h"

namespace Can::graphics::d3d12::content
{
	bool initialize();
	void shutdown();

	namespace submesh
	{
		id::id_type add(const u8*& data);
		void remove(id::id_type id);
	}

	namespace texture
	{
		id::id_type add(const u8* const data);
		void remove(id::id_type id);
		void get_descriptor_indices(const id::id_type* const texture_ids, u32 id_count, u32* const indices);
	}

	namespace material
	{
		id::id_type add(material_init_info info);
		void remove(id::id_type id);
	}
}