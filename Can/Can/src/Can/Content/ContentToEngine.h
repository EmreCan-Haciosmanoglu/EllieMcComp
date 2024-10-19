#pragma once
#include "Can/Common/CommonHeaders.h"
#include "Can/Core.h"
namespace Can::content
{
	struct primitive_topology
	{
		enum type : u32 {
			point_List = 1,
			line_list,
			line_strip,
			triangle_list,
			triangle_strip,

			count
		};
	};

	struct lod_offset {
		u16 offset;
		u16 count;
	};
}