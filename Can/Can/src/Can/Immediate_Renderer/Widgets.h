#pragma once

#include "Can/Renderer/RenderCommand.h"
#include "Can/Renderer/Shader.h"
#include "Can/Renderer/Texture.h"

#define MAXIMUM_QUAD_COUNT  10000
#define MAXIMUM_VERTICES  MAXIMUM_QUAD_COUNT * 4
#define MAXIMUM_INDECIES  MAXIMUM_QUAD_COUNT * 6
#define MAXIMUM_TEXTURE_SLOTS 32

namespace Can
{
	class Font;
	class FontAtlas;

	struct Vertex
	{
		v3 position;
		v4 tint_color;
		v2 uv;
		f32 texture_index;
		f32 tiling_factor;
	};

	struct Buffer_Data_Font_Atlas_Key
	{
		Font* font = nullptr;
		u16 font_size_in_pixel = 1;
	};

	struct Buffer_Data_Font_Atlas_Key_Value_Pair {
		Buffer_Data_Font_Atlas_Key key;
		FontAtlas* font_atlas;	// value
	};

	struct Buffer_Data
	{
		Ref<VertexArray> vertex_array;
		Ref<VertexBuffer> vertex_buffer;
		Ref<Shader> shader;
		//Ref<Texture2D> white_texture;

		u64 quad_count = 0;

		Vertex buffer_base[MAXIMUM_VERTICES];
		u64 buffer_cursor = 0;

		Ref<Texture2D> texture_slots[MAXIMUM_TEXTURE_SLOTS];
		u64 texture_slots_cursor = 1; // 0 => white texture;

		u16 default_font_size_in_pixel = 24;
		Font* default_font;
		std::vector<Buffer_Data_Font_Atlas_Key_Value_Pair> font_atlas{};
		Orthographic_Camera_Controller* camera_controller = nullptr;
	};

	struct Rect
	{
		s32 x = 0;
		s32 y = 0;
		s32 z = 0;
		s32 w = 1;
		s32 h = 1;
	};
}