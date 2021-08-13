#pragma once

#include "Can/Renderer/RenderCommand.h"
#include "Can/Renderer/Shader.h"
#include "Can/Renderer/Texture.h"
//#include "Can/Font/Font.h"
//#include "Can/Font/FontAtlas.h"

#include "Can/Camera/Orthographic_Camera_Controller.h"

#define MAXIMUM_QUAD_COUNT  10000
#define MAXIMUM_VERTICES  MAXIMUM_QUAD_COUNT * 4
#define MAXIMUM_INDECIES  MAXIMUM_QUAD_COUNT * 6
#define MAXIMUM_TEXTURE_SLOTS 32

namespace Can
{
	struct Vertex
	{
		v3 position;
		v4 tint_color;
		v2 uv;
		f32 texture_index;
		f32 tiling_factor;
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

		//Font* font;
		//std::vector<FontAtlas> font_atlas{};
		Orthographic_Camera_Controller* camera_controller = nullptr;
	};

	struct Rect
	{
		f32 x = 1.0f;
		f32 y = 1.0f;
		f32 w = 1.0f;
		f32 h = 1.0f;
	};

	void init_immediate_renderer();

	void set_camera_for_immediate_renderer();

	void immediate_quad(Rect& r, v4& color);

	void immediate_flush();

}