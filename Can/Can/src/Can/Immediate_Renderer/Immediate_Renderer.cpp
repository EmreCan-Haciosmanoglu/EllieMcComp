#include "canpch.h"

#include "Immediate_Renderer.h"

#include "Can/EntryPoint.h"
#include "Can/Hash_Table.h"

#include "Can/Font/Font.h"
#include "Can/Font/FontAtlas.h"

#include "ft2build.h"
#include FT_FREETYPE_H

#include "Can/Font/FontFlags.h"
#include "Can/Math.h"

#undef max
#undef min

namespace Can
{

	Buffer_Data buffer_data;

	Hash_Table<Button_State, 0xffff> button_states;
	Hash_Table<Drop_Down_List_State, 0xffff> drop_down_list_states;
	Hash_Table<Slider_State, 0xffff> slider_states;
	Hash_Table<Sub_Region_State, 0xffff> sub_region_states;
	Hash_Table<Check_Box_State, 0xffff> check_box_states;
	Hash_Table<Text_Box_State, 0xffff> text_box_states;

#define SOME_DENOM 100.0f
	void update_used_region(Rect& rect)
	{
		std::vector<Buffer_Data::Sub_Region>& regions = buffer_data.sub_regions;
		u64 region_count = buffer_data.sub_region_count;
		if (region_count)
		{
			Rect& used_region = regions[region_count - 1].used_region;
			Rect& region = regions[region_count - 1].region;

			s32 rx = region.x + rect.x;
			s32 ry = region.y + rect.y;

			s32 ux2 = used_region.x + used_region.w;
			s32 uy2 = used_region.y + used_region.h;
			ux2 = std::max(ux2, rx + rect.w);
			uy2 = std::max(uy2, ry + rect.h);

			used_region.x = std::min(used_region.x, rx);
			used_region.y = std::min(used_region.y, ry);
			used_region.w = ux2 - used_region.x;
			used_region.h = uy2 - used_region.y;
		}
	}
	Rect clip_with_regions(Rect& rect)
	{
		Rect new_rect = rect;
		std::vector<Buffer_Data::Sub_Region>& regions = buffer_data.sub_regions;
		u64 index = buffer_data.sub_region_count;
		while (true)
		{
			if (index == 0)	break;
			index--;

			Buffer_Data::Sub_Region& sub_region = regions[index];
			Sub_Region_State* state = get_or_init(sub_region_states, sub_region.hash);

			s32 rx1 = sub_region.region.x;
			s32 rx2 = sub_region.region.x + sub_region.region.w;
			s32 ry1 = sub_region.region.y;
			s32 ry2 = sub_region.region.y + sub_region.region.h;

			s32 ux1 = sub_region.prev_frame_used_region.x;
			s32 ux2 = sub_region.prev_frame_used_region.x + sub_region.prev_frame_used_region.w;
			s32 uy1 = sub_region.prev_frame_used_region.y;
			s32 uy2 = sub_region.prev_frame_used_region.y + sub_region.prev_frame_used_region.h;

			s32 scrolled_x_diff = Math::lerp(rx1 - ux1, rx2 - ux2, state->horizontal_scroll_percentage);
			s32 scrolled_y_diff = Math::lerp(ry1 - uy1, ry2 - uy2, 1.0f - state->vertical_scroll_percentage);

			if (sub_region.theme->flags & SUB_REGION_THEME_FLAGS_HORIZONTALLY_SCROLLABLE)
				new_rect.x += scrolled_x_diff;
			if (sub_region.theme->flags & SUB_REGION_THEME_FLAGS_VERTICALLY_SCROLLABLE)
				new_rect.y += scrolled_y_diff;

			s32 nrx2 = new_rect.x + new_rect.w;
			s32 nry2 = new_rect.y + new_rect.h;

			new_rect.x = std::max(0, new_rect.x);
			new_rect.y = std::max(0, new_rect.y);

			nrx2 = std::min(nrx2, sub_region.region.w);
			nry2 = std::min(nry2, sub_region.region.h);

			new_rect.w = nrx2 - new_rect.x;
			new_rect.h = nry2 - new_rect.y;

			if (new_rect.w <= 0 || new_rect.h <= 0) return new_rect;

			new_rect.x += sub_region.region.x;
			new_rect.y += sub_region.region.y;

		}
		return new_rect;
	}

	void init_immediate_renderer()
	{
		buffer_data.vertex_buffer = VertexBuffer::Create(MAXIMUM_VERTICES * sizeof(Vertex));
		buffer_data.vertex_buffer->SetLayout({
			{ShaderDataType::Float3, "a_Position"},
			{ShaderDataType::Float4, "a_TintColor"},
			{ShaderDataType::Float2, "a_TexCoord"},
			{ShaderDataType::Float, "a_TexIndex"},
			{ShaderDataType::Float, "a_TilingFactor"}
			});

		buffer_data.vertex_array = VertexArray::Create();
		buffer_data.vertex_array->AddVertexBuffer(buffer_data.vertex_buffer);

		u32* indices = new u32[MAXIMUM_INDECIES];

		uint32_t offset = 0;
		for (size_t i = 0; i < MAXIMUM_INDECIES; i += 6)
		{
			indices[i + 0] = offset + 0;
			indices[i + 1] = offset + 1;
			indices[i + 2] = offset + 2;
			indices[i + 3] = offset + 2;
			indices[i + 4] = offset + 3;
			indices[i + 5] = offset + 0;

			offset += 4;
		}
		Ref<IndexBuffer> index_buffer = IndexBuffer::Create(indices, MAXIMUM_INDECIES);
		buffer_data.vertex_array->SetIndexBuffer(index_buffer);

		buffer_data.shader = Shader::Create("assets/shaders/2DTexture.glsl");
		buffer_data.texture_slots[0] = Texture2D::Create(1, 1);

		u32 white_dot = 0xffffffff;
		buffer_data.texture_slots[0]->SetData(&white_dot, sizeof(white_dot));

		s32 samplers[MAXIMUM_TEXTURE_SLOTS];
		for (uint32_t i = 0; i < MAXIMUM_TEXTURE_SLOTS; i++)
			samplers[i] = i;

		buffer_data.shader->Bind();
		buffer_data.shader->SetIntArray("u_Textures", samplers, MAXIMUM_TEXTURE_SLOTS);

		buffer_data.default_font = new Font("assets/Fonts/Poppins/Poppins-Medium.ttf");
		Buffer_Data_Font_Atlas_Key key;
		key.font_size_in_pixel = 24;
		key.font = buffer_data.default_font;
		Buffer_Data_Font_Atlas_Key_Value_Pair key_value_pair{
			key,
			new FontAtlas(buffer_data.default_font->face, key.font_size_in_pixel)
		};
		buffer_data.font_atlas.push_back(key_value_pair);
		delete[] indices;
	}

	void set_camera_for_immediate_renderer()
	{
		auto& camera = buffer_data.camera_controller->m_Camera;
		buffer_data.shader->Bind();
		buffer_data.shader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
	}

	void immediate_quad(v2& p0, v2& p1, v2& p2, v2& p3, v2& uv0, v2& uv1, v2& uv2, v2& uv3, v4& color)
	{
		immediate_quad(p0, p1, p2, p3, uv0, uv1, uv2, uv3, color, 0.0f);
	}
	void immediate_quad(v2i& p0i, v2i& p1i, v2i& p2i, v2i& p3i, v2& uv0, v2& uv1, v2& uv2, v2& uv3, v4& color, f32 texture_index)
	{
		immediate_quad(v3i(p0i, 0), v3i(p1i, 0), v3i(p2i, 0), v3i(p3i, 0), uv0, uv1, uv2, uv3, color, texture_index);
	}
	void immediate_quad(v3i& p0i, v3i& p1i, v3i& p2i, v3i& p3i, v2& uv0, v2& uv1, v2& uv2, v2& uv3, v4& color, f32 texture_index)
	{
		auto& window = main_application->GetWindow();

		u64 width_in_pixels = window.GetWidth();
		u64 height_in_pixels = window.GetHeight();

		f32 aspect_ratio = buffer_data.camera_controller->m_AspectRatio;
		f32 zoom_level = buffer_data.camera_controller->m_ZoomLevel;

		f32 width_float = 2.0f * aspect_ratio * zoom_level;
		f32 height_float = 2.0f * zoom_level;

		v3 p0 = {
			(((p0i.x * 1.0f) / width_in_pixels) - 0.5f) * width_float,
			(((p0i.y * 1.0f) / height_in_pixels) - 0.5f) * height_float,
			p0i.z / SOME_DENOM
		};
		v3 p1 = {
			(((p1i.x * 1.0f) / width_in_pixels) - 0.5f) * width_float,
			(((p1i.y * 1.0f) / height_in_pixels) - 0.5f) * height_float,
			p1i.z / SOME_DENOM
		};
		v3 p2 = {
			(((p2i.x * 1.0f) / width_in_pixels) - 0.5f) * width_float,
			(((p2i.y * 1.0f) / height_in_pixels) - 0.5f) * height_float,
			p2i.z / SOME_DENOM
		};
		v3 p3 = {
			(((p3i.x * 1.0f) / width_in_pixels) - 0.5f) * width_float,
			(((p3i.y * 1.0f) / height_in_pixels) - 0.5f) * height_float,
			p3i.z / SOME_DENOM
		};
		immediate_quad(p0, p1, p2, p3, uv0, uv1, uv2, uv3, color, texture_index);
	}
	void immediate_quad(v2& p0, v2& p1, v2& p2, v2& p3, v2& uv0, v2& uv1, v2& uv2, v2& uv3, v4& color, f32 texture_index)
	{
		immediate_quad(v3(p0, 0.0f), v3(p1, 0.0f), v3(p2, 0.0f), v3(p3, 0.0f), uv0, uv1, uv2, uv3, color, texture_index);
	}
	void immediate_quad(v3& p0, v3& p1, v3& p2, v3& p3, v2& uv0, v2& uv1, v2& uv2, v2& uv3, v4& color, f32 texture_index)
	{
		Vertex vertex;
		vertex.position = p0;
		vertex.uv = uv0;
		vertex.tint_color = color;
		vertex.texture_index = texture_index;
		vertex.tiling_factor = 1.0f;

		buffer_data.buffer_base[buffer_data.buffer_cursor] = vertex;
		buffer_data.buffer_cursor++;

		vertex.position = p1;
		vertex.uv = uv1;
		buffer_data.buffer_base[buffer_data.buffer_cursor] = vertex;
		buffer_data.buffer_cursor++;

		vertex.position = p2;
		vertex.uv = uv2;
		buffer_data.buffer_base[buffer_data.buffer_cursor] = vertex;
		buffer_data.buffer_cursor++;

		vertex.position = p3;
		vertex.uv = uv3;
		buffer_data.buffer_base[buffer_data.buffer_cursor] = vertex;
		buffer_data.buffer_cursor++;


		buffer_data.quad_count += 6;
	}
	void immediate_quad(v2& p0, v2& p1, v2& p2, v2& p3, v4& color)
	{
		immediate_quad(v3(p0, 0.0f), v3(p1, 0.0f), v3(p2, 0.0f), v3(p3, 0.0f), color);
	}
	void immediate_quad(v3& p0, v3& p1, v3& p2, v3& p3, v4& color)
	{
		Vertex vertex;
		vertex.position = p0;
		vertex.uv = { 0.0f, 0.0f };
		vertex.tint_color = color;
		vertex.texture_index = 0.0f;
		vertex.tiling_factor = 1.0f;

		buffer_data.buffer_base[buffer_data.buffer_cursor] = vertex;
		buffer_data.buffer_cursor++;

		vertex.position = p1;
		vertex.uv = { 1.0f, 0.0f };
		buffer_data.buffer_base[buffer_data.buffer_cursor] = vertex;
		buffer_data.buffer_cursor++;

		vertex.position = p2;
		vertex.uv = { 1.0f, 1.0f };
		buffer_data.buffer_base[buffer_data.buffer_cursor] = vertex;
		buffer_data.buffer_cursor++;

		vertex.position = p3;
		vertex.uv = { 0.0f, 1.0f };
		buffer_data.buffer_base[buffer_data.buffer_cursor] = vertex;
		buffer_data.buffer_cursor++;


		buffer_data.quad_count += 6;
	}
	void immediate_quad(v2i& p0i, v2i& p1i, v2i& p2i, v2i& p3i, v4& color)
	{
		immediate_quad(v3i(p0i, 0), v3i(p1i, 0), v3i(p2i, 0), v3i(p3i, 0), color);
	}
	void immediate_quad(v3i& p0i, v3i& p1i, v3i& p2i, v3i& p3i, v4& color)
	{
		auto& window = main_application->GetWindow();

		u64 width_in_pixels = window.GetWidth();
		u64 height_in_pixels = window.GetHeight();

		f32 aspect_ratio = buffer_data.camera_controller->m_AspectRatio;
		f32 zoom_level = buffer_data.camera_controller->m_ZoomLevel;

		f32 width_float = 2.0f * aspect_ratio * zoom_level;
		f32 height_float = 2.0f * zoom_level;

		v3 p0 = {
			(((p0i.x * 1.0f) / width_in_pixels) - 0.5f) * width_float,
			(((p0i.y * 1.0f) / height_in_pixels) - 0.5f) * height_float,
			p0i.z / SOME_DENOM
		};
		v3 p1 = {
			(((p1i.x * 1.0f) / width_in_pixels) - 0.5f) * width_float,
			(((p1i.y * 1.0f) / height_in_pixels) - 0.5f) * height_float,
			p1i.z / SOME_DENOM
		};
		v3 p2 = {
			(((p2i.x * 1.0f) / width_in_pixels) - 0.5f) * width_float,
			(((p2i.y * 1.0f) / height_in_pixels) - 0.5f) * height_float,
			p2i.z / SOME_DENOM
		};
		v3 p3 = {
			(((p3i.x * 1.0f) / width_in_pixels) - 0.5f) * width_float,
			(((p3i.y * 1.0f) / height_in_pixels) - 0.5f) * height_float,
			p3i.z / SOME_DENOM
		};
		immediate_quad(p0, p1, p2, p3, color);
	}
	void immediate_quad(Rect& rect, v4& color, bool relative)
	{
		Rect r = rect;
		if (relative)
		{
			update_used_region(rect);
			r = clip_with_regions(rect);
			if (r.w <= 0 || r.h <= 0) return;
		}
		v3i p0i{ r.x,       r.y,       r.z };
		v3i p1i{ r.x + r.w, r.y,       r.z };
		v3i p2i{ r.x + r.w, r.y + r.h, r.z };
		v3i p3i{ r.x,       r.y + r.h, r.z };
		immediate_quad(p0i, p1i, p2i, p3i, color);
	}

	FontAtlas* find_or_init_value(Buffer_Data_Font_Atlas_Key key)
	{
		auto& atlases = buffer_data.font_atlas;
		for (u64 i = 0; i < atlases.size(); i++)
		{
			Buffer_Data_Font_Atlas_Key_Value_Pair& pair = atlases[i];
			if (key.font_size_in_pixel == pair.key.font_size_in_pixel && key.font == pair.key.font)
				return pair.font_atlas;
		}
		Buffer_Data_Font_Atlas_Key_Value_Pair pair{
			key,
			new FontAtlas(key.font->face, key.font_size_in_pixel)
		};
		atlases.push_back(pair);
		return pair.font_atlas;
	}
	void immediate_text(const std::string& text, Rect& rect, Label_Theme& theme, bool relative)
	{
		Font* font = theme.font;
		u16 font_size_in_pixel = theme.font_size_in_pixel;
		if (!font)
		{
			font = buffer_data.default_font;
			font_size_in_pixel = buffer_data.default_font_size_in_pixel;
		}
		FontAtlas* atlas = find_or_init_value({ font, font_size_in_pixel });
		FT_Set_Pixel_Sizes(font->face, 0, font_size_in_pixel);

		// Calculate alignment (if applicable)
		s32 text_width = 0;
		Char* chars = atlas->chars;
		for (const s8* c = text.c_str(); *c; ++c)
		{
			u8 p = (u8)*c;
			text_width += chars[p].advanceX;
		}

		s32 atlas_width = atlas->width;
		s32 atlas_height = atlas->height;

		Rect region = rect;
		if (relative)
		{
			update_used_region(rect);
			region = clip_with_regions(rect);
			if (region.w <= 0 || region.h <= 0) return;
		}

		v3i pos{ region.x + region.w / 2 - text_width / 2, region.y + region.h / 2 - atlas_height / 2, region.z };

		if (theme.flags & FontFlags::LeftAligned)
		{
			pos.x = region.x;
		}
		else if (theme.flags & FontFlags::RightAligned)
		{
			pos.x = region.x + region.w - text_width;
		}

		if (theme.flags & FontFlags::TopAligned)
		{
			pos.y = region.y + region.h - atlas_height;
		}
		else if (theme.flags & FontFlags::BottomAligned)
		{
			pos.y = region.y;
		}

		pos.y += font->face->size->metrics.height >> 6;
		pos.y -= atlas_height;


		f32 texture_index = 0.0f;

		for (u64 i = 1; i < buffer_data.texture_slots_cursor; i++)
		{
			atlas->texture.get();
			if (*buffer_data.texture_slots[i].get() == *atlas->texture.get())
			{
				texture_index = (float)i;
				break;
			}
		}
		if (texture_index == 0.0f)
		{
			texture_index = (f32)buffer_data.texture_slots_cursor;
			buffer_data.texture_slots[buffer_data.texture_slots_cursor] = atlas->texture;
			buffer_data.texture_slots_cursor++;
		}

		for (const char* c = text.c_str(); *c; ++c)
		{
			u8 p = *c;
			s32 x = pos.x + chars[p].bitmapLeft;
			s32 y = pos.y + chars[p].bitmapTop;
			s32 z = pos.z;
			s32 w = chars[p].bitmapWidth;
			s32 h = chars[p].bitmapHeight;

			v3i p0i{ x,     y - h, z };
			v3i p1i{ x + w, y - h, z };
			v3i p2i{ x + w, y,     z };
			v3i p3i{ x,     y,     z };

			v2 uv0{ chars[p].xOffset, chars[p].bitmapHeight / atlas_height };
			v2 uv1{ chars[p].xOffset + chars[p].bitmapWidth / atlas_width, chars[p].bitmapHeight / atlas_height };
			v2 uv2{ chars[p].xOffset + chars[p].bitmapWidth / atlas_width, 0.0f };
			v2 uv3{ chars[p].xOffset, 0.0f };

			// Calculate kerning value
			FT_Vector kerning;
			FT_Get_Kerning(
				font->face,	// font face handle
				p,								// left glyph
				p + 1,						// right glyph
				FT_KERNING_DEFAULT,				// kerning mode
				&kerning);						// variable to store kerning value

			pos.x += chars[p].advanceX + (kerning.x >> 6);
			pos.y += chars[p].advanceY;
			pos.z++;

			if (w == 0 || h == 0)
				continue;

			immediate_quad(p0i, p1i, p2i, p3i, uv0, uv1, uv2, uv3, theme.color, texture_index);
			//immediate_quad(p0i, p1i, p2i, p3i, uv0, uv1, uv2, uv3, color, 0.0f);
		}
	}

	bool global_pressed = false;
	u64 pressed_hash = 0;
	bool inside(Rect& r, s32 x, s32 y) { return x >= r.x && x <= r.x + r.w && y >= r.y && y <= r.y + r.h; }
	u16 immediate_button(Rect& rect, const std::string& text, Button_Theme& theme, u64 hash, bool relative)
	{
		Button_State* state = get_or_init(button_states, hash);

		bool mouse_pressed = Input::IsMouseButtonPressed(MouseCode::Button0);
		auto window_heigth = main_application->GetWindow().GetHeight();
		auto [mouse_x, mouse_y] = Input::get_mouse_pos();
		mouse_y = window_heigth - mouse_y;
		Rect region = rect;
		if (relative)
		{
			update_used_region(rect);
			region = clip_with_regions(rect);
		}
		{
			v3i p0i{ region.x,            region.y,            region.z };
			v3i p1i{ region.x + region.w, region.y,            region.z };
			v3i p2i{ region.x + region.w, region.y + region.h, region.z };
			v3i p3i{ region.x,            region.y + region.h, region.z };
			v4 color = theme.background_color;
			if (!global_pressed || (global_pressed && pressed_hash == hash))
			{
				bool mouse_over = inside(region, mouse_x, mouse_y);
				if (mouse_over)
				{
					state->flags |= BUTTON_STATE_FLAGS_OVER;
					color = theme.background_color_over;

					if (mouse_pressed)
					{
						global_pressed = true;
						pressed_hash = hash;

						if (state->flags & BUTTON_STATE_FLAGS_PRESSED)
						{
							state->flags &= (0xffff ^ BUTTON_STATE_FLAGS_PRESSED);
							state->flags |= BUTTON_STATE_FLAGS_HOLD;
						}
						else
						{
							if (!(state->flags & BUTTON_STATE_FLAGS_HOLD))
							{
								state->flags |= BUTTON_STATE_FLAGS_PRESSED;
							}
						}
						color = theme.background_color_pressed;
					}
					else
					{
						global_pressed = false;
						if (state->flags & BUTTON_STATE_FLAGS_PRESSED || state->flags & BUTTON_STATE_FLAGS_HOLD)
						{
							state->flags &= (0xffff ^ BUTTON_STATE_FLAGS_PRESSED);
							state->flags &= (0xffff ^ BUTTON_STATE_FLAGS_HOLD);
							state->flags |= BUTTON_STATE_FLAGS_RELEASED;
						}
						else
						{
							state->flags &= (0xffff ^ BUTTON_STATE_FLAGS_RELEASED);
						}
					}
				}
				else
				{
					state->flags &= (0xffff ^ BUTTON_STATE_FLAGS_OVER);

					if (!mouse_pressed)
					{
						if (state->flags & BUTTON_STATE_FLAGS_PRESSED || state->flags & BUTTON_STATE_FLAGS_HOLD)
						{
							state->flags &= (0xffff ^ BUTTON_STATE_FLAGS_PRESSED);
							state->flags &= (0xffff ^ BUTTON_STATE_FLAGS_HOLD);
							state->flags |= BUTTON_STATE_FLAGS_RELEASED;
						}
						else
						{
							state->flags &= (0xffff ^ BUTTON_STATE_FLAGS_RELEASED);
						}
						global_pressed = false;
						color = theme.background_color;
					}
					else
					{
						if (state->flags & BUTTON_STATE_FLAGS_PRESSED || state->flags & BUTTON_STATE_FLAGS_HOLD)
							color == theme.background_color_pressed;
					}
				}
			}
			immediate_quad(p0i, p1i, p2i, p3i, color);
			Rect text_rect = rect;
			text_rect.z++;
			immediate_text(text, text_rect, *theme.label_theme);
		}
		return state->flags;
	}
	u16 immediate_drop_down_list(Rect& rect, std::vector<std::string>& list, u64& selected_item, Drop_Down_List_Theme& theme, u64 hash)
	{
		bool first_time = false;
		Drop_Down_List_State* state = get_or_init(drop_down_list_states, hash, first_time);
		if (first_time) state->active_item = selected_item;

		/*
		std::vector<Buffer_Data::Sub_Region>& regions = buffer_data.sub_regions;
		u64 region_count = regions.size();
		if (region_count)
		{
			Rect& used_region = regions[region_count - 1].used_region;
			used_region.x = std::min(used_region.x, rect.x);
			used_region.y = std::min(used_region.y, rect.y);
			used_region.w = std::max(used_region.x + used_region.w, rect.x + rect.w) - used_region.x;
			used_region.h = std::max(used_region.y + used_region.h, rect.y + rect.h) - used_region.y;
		}

		Rect region = rect;
		for (u64 i = 0; i < region_count; i++)
		{
			Rect& reg = regions[i].region;

			region.x += reg.x;
			region.y += reg.y;
			region.z += reg.z;
			s32 xlim = std::min(reg.x + reg.w, region.x + region.w);
			s32 ylim = std::min(reg.y + reg.h, region.y + region.h);
			s32 w = xlim - region.x;
			s32 h = ylim - region.y;
			if (w <= 0 || h <= 0)
			{
				state->flags |= BUTTON_STATE_FLAGS_FULLY_HIDDEN;
				break;
			}
			region.w = w;
			region.h = h;

		}
		*/
		state->flags &= (0xffff ^ DROP_DOWN_LIST_STATE_FLAGS_ITEM_CHANGED);

		bool mouse_pressed = Input::IsMouseButtonPressed(MouseCode::Button0);
		auto window_heigth = main_application->GetWindow().GetHeight();
		auto [mouse_x, mouse_y] = Input::get_mouse_pos();
		mouse_y = window_heigth - mouse_y;
		bool over_children = false;
		{
			if (state->flags & DROP_DOWN_LIST_STATE_FLAGS_OPEN)
			{
				Rect lr = rect;
				u64 count = list.size();
				for (u64 i = 0; i < count; i++)
				{
					lr.y -= lr.h;
					Button_Theme* button_theme = theme.button_theme;
					bool selected = (i == state->active_item);
					if (selected)
						button_theme = theme.button_theme_selected;

					u64 button_hash = ((i + 1) << 32) + hash;
					u16 button_flags = immediate_button(lr, list[i], *button_theme, button_hash);
					if (button_flags & BUTTON_STATE_FLAGS_OVER)
						over_children = true;
					if (!selected && (button_flags & BUTTON_STATE_FLAGS_RELEASED))
					{
						state->active_item = i;
						state->flags |= DROP_DOWN_LIST_STATE_FLAGS_ITEM_CHANGED;
						state->flags &= (0xffff ^ DROP_DOWN_LIST_STATE_FLAGS_OPEN);

						selected_item = i;
					}
				}
			}
		}
		{
			u16 flags = immediate_button(rect, list[state->active_item], *theme.button_theme, hash);
			if (flags & BUTTON_STATE_FLAGS_RELEASED)
				state->flags ^= DROP_DOWN_LIST_STATE_FLAGS_OPEN;
			state->flags &= 0xfff0;
			state->flags |= flags;
		}
		return state->flags;
	}
	u16 immediate_slider(Rect& track_rect, Rect& thumb_rect, std::string& text, s16 min_value, s16& current_value, s16 max_value, Slider_Theme& theme, u64 hash)
	{
		Slider_State* state = get_or_init(slider_states, hash);
		//assert(max - min >= 0.0f); // start using these
		bool mouse_pressed = Input::IsMouseButtonPressed(MouseCode::Button0);
		auto window_heigth = main_application->GetWindow().GetHeight();
		auto [mouse_x, mouse_y] = Input::get_mouse_pos();
		mouse_y = window_heigth - mouse_y;

		Rect regioned_track_rect = track_rect;
		Rect regioned_thumb_rect = thumb_rect;

		{
			Button_Theme& track_theme = *theme.track_theme;
			v3i p0i{ regioned_track_rect.x,                         regioned_track_rect.y,                         regioned_track_rect.z };
			v3i p1i{ regioned_track_rect.x + regioned_track_rect.w, regioned_track_rect.y,                         regioned_track_rect.z };
			v3i p2i{ regioned_track_rect.x + regioned_track_rect.w, regioned_track_rect.y + regioned_track_rect.h, regioned_track_rect.z };
			v3i p3i{ regioned_track_rect.x,                         regioned_track_rect.y + regioned_track_rect.h, regioned_track_rect.z };
			v4 color = track_theme.background_color;
			immediate_quad(p0i, p1i, p2i, p3i, color);
		}
		{
			Button_Theme& thumb_theme = *theme.thumb_theme;
			regioned_thumb_rect.z = regioned_track_rect.z + 1;

			f32 denom = max_value - min_value;
			if (denom <= 0.0f) denom = 1.0f;
			f32 ratio = (current_value - min_value) / denom;
			if (theme.flags & SLIDER_THEME_FLAGS_IS_HORIZONTAL)
			{
				s32 min_x = regioned_track_rect.x - regioned_thumb_rect.w / 2;
				s32 max_x = regioned_track_rect.x + regioned_track_rect.w - regioned_thumb_rect.w / 2;
				if (theme.flags & SLIDER_THEME_FLAGS_THUMB_IS_INSIDE)
				{
					min_x += regioned_thumb_rect.w / 2;
					max_x -= regioned_thumb_rect.w / 2;
				}
				regioned_thumb_rect.x = Math::lerp(min_x, max_x, ratio);
			}
			else
			{
				s32 min_y = regioned_track_rect.y + regioned_track_rect.h - regioned_thumb_rect.h / 2;
				s32 max_y = regioned_track_rect.y - regioned_thumb_rect.h / 2;
				if (theme.flags & SLIDER_THEME_FLAGS_THUMB_IS_INSIDE)
				{
					min_y -= regioned_thumb_rect.h / 2;
					max_y += regioned_thumb_rect.h / 2;
				}
				regioned_thumb_rect.y = Math::lerp(min_y, max_y, ratio);
			}

			if (theme.flags & SLIDER_THEME_FLAGS_IS_HORIZONTAL)
			{
				regioned_thumb_rect.y = regioned_track_rect.y;
				if (theme.flags & SLIDER_THEME_FLAGS_THUMB_AT_TOP_OR_RIGHT)
					regioned_thumb_rect.y += regioned_track_rect.h + theme.x_or_y_offset_in_pixels;
				else if (theme.flags & SLIDER_THEME_FLAGS_THUMB_AT_BOTTOM_OR_LEFT)
					regioned_thumb_rect.y -= regioned_thumb_rect.h + theme.x_or_y_offset_in_pixels;
				else
					regioned_thumb_rect.y += regioned_track_rect.h / 2 - regioned_thumb_rect.h / 2;
			}
			else
			{
				regioned_thumb_rect.x = regioned_track_rect.x;
				if (theme.flags & SLIDER_THEME_FLAGS_THUMB_AT_TOP_OR_RIGHT)
					regioned_thumb_rect.x += regioned_track_rect.w + theme.x_or_y_offset_in_pixels;
				else if (theme.flags & SLIDER_THEME_FLAGS_THUMB_AT_BOTTOM_OR_LEFT)
					regioned_thumb_rect.x -= regioned_thumb_rect.w + theme.x_or_y_offset_in_pixels;
				else
					regioned_thumb_rect.x += regioned_track_rect.w / 2 - regioned_thumb_rect.w / 2;
			}

			v4 color = thumb_theme.background_color;
			if (!global_pressed || (global_pressed && pressed_hash == hash))
			{
				bool mouse_over_track = inside(regioned_track_rect, mouse_x, mouse_y);
				bool mouse_over_thumb = inside(regioned_thumb_rect, mouse_x, mouse_y);

				if (mouse_over_track || mouse_over_thumb)
				{
					state->flags |= SLIDER_STATE_FLAGS_OVER;
					color = thumb_theme.background_color_over;

					if (mouse_pressed)
					{
						global_pressed = true;
						pressed_hash = hash;

						if (state->flags & SLIDER_STATE_FLAGS_THUMB_PRESSED)
						{
							state->flags &= (0xffff ^ SLIDER_STATE_FLAGS_THUMB_PRESSED);
							state->flags |= SLIDER_STATE_FLAGS_THUMB_HOLD;
						}
						else
						{
							if (!(state->flags & SLIDER_STATE_FLAGS_THUMB_HOLD))
							{
								state->flags |= SLIDER_STATE_FLAGS_THUMB_PRESSED;
							}
						}
						color = thumb_theme.background_color_pressed;
					}
					else
					{
						global_pressed = false;
						if (state->flags & SLIDER_STATE_FLAGS_THUMB_PRESSED || state->flags & SLIDER_STATE_FLAGS_THUMB_HOLD)
						{
							state->flags &= (0xffff ^ SLIDER_STATE_FLAGS_THUMB_PRESSED);
							state->flags &= (0xffff ^ SLIDER_STATE_FLAGS_THUMB_HOLD);
							state->flags |= SLIDER_STATE_FLAGS_THUMB_RELEASED;
						}
						else
						{
							state->flags &= (0xffff ^ SLIDER_STATE_FLAGS_THUMB_RELEASED);
						}
					}
				}
				else
				{
					state->flags &= (0xffff ^ SLIDER_STATE_FLAGS_OVER);

					if (!mouse_pressed)
					{
						if (state->flags & SLIDER_STATE_FLAGS_THUMB_PRESSED || state->flags & SLIDER_STATE_FLAGS_THUMB_HOLD)
						{
							state->flags &= (0xffff ^ SLIDER_STATE_FLAGS_THUMB_PRESSED);
							state->flags &= (0xffff ^ SLIDER_STATE_FLAGS_THUMB_HOLD);
							state->flags |= SLIDER_STATE_FLAGS_THUMB_RELEASED;
						}
						else
						{
							state->flags &= (0xffff ^ SLIDER_STATE_FLAGS_THUMB_RELEASED);
						}
						global_pressed = false;
						color = thumb_theme.background_color;
					}
					else
					{
						if (state->flags & BUTTON_STATE_FLAGS_PRESSED || state->flags & BUTTON_STATE_FLAGS_HOLD)
							color == thumb_theme.background_color_pressed;
					}
				}
			}

			if ((state->flags & SLIDER_STATE_FLAGS_THUMB_HOLD) || (state->flags & SLIDER_STATE_FLAGS_THUMB_PRESSED))
			{
				if (theme.flags & SLIDER_THEME_FLAGS_IS_HORIZONTAL)
				{
					f32 denom = regioned_track_rect.w;
					if (denom <= 0.0f) denom = 1.0f;
					f32 ratio = ((s32)mouse_x - regioned_track_rect.x) / denom;
					current_value = min_value + ratio * (max_value - min_value);
				}
				else
				{
					f32 denom = regioned_track_rect.h;
					if (denom <= 0.0f) denom = 1.0f;
					f32 ratio = ((regioned_track_rect.y + regioned_track_rect.h) - (s32)mouse_y) / denom;
					current_value = min_value + ratio * (max_value - min_value);
				}
			}

			if (theme.flags & SLIDER_THEME_FLAGS_CLAMP_VALUE)
			{
				current_value = std::max(min_value, current_value);
				current_value = std::min(max_value, current_value);
			}

			v3i p0i{ regioned_thumb_rect.x,                         regioned_thumb_rect.y,                         regioned_thumb_rect.z };
			v3i p1i{ regioned_thumb_rect.x + regioned_thumb_rect.w, regioned_thumb_rect.y,                         regioned_thumb_rect.z };
			v3i p2i{ regioned_thumb_rect.x + regioned_thumb_rect.w, regioned_thumb_rect.y + regioned_thumb_rect.h, regioned_thumb_rect.z };
			v3i p3i{ regioned_thumb_rect.x,                         regioned_thumb_rect.y + regioned_thumb_rect.h, regioned_thumb_rect.z };
			immediate_quad(p0i, p1i, p2i, p3i, color);

			if (theme.flags & SLIDER_THEME_FLAGS_VALUE_SHOWN)
			{
				Rect text_rect = regioned_thumb_rect;
				s32 thumb_margin = 5;

				if (theme.flags & SLIDER_THEME_FLAGS_IS_HORIZONTAL)
				{
					if (theme.flags & SLIDER_THEME_FLAGS_THUMB_AT_TOP_OR_RIGHT)
						text_rect.y += regioned_thumb_rect.h + thumb_margin;
					else if (theme.flags & SLIDER_THEME_FLAGS_THUMB_AT_BOTTOM_OR_LEFT)
						text_rect.y -= theme.label_theme->font_size_in_pixel + thumb_margin;
					else
						text_rect.y = regioned_track_rect.y + std::max(regioned_track_rect.h, regioned_thumb_rect.h) + thumb_margin;
				}
				else
				{
					if (theme.flags & SLIDER_THEME_FLAGS_THUMB_AT_TOP_OR_RIGHT)
						text_rect.x += regioned_thumb_rect.w + thumb_margin;
					else if (theme.flags & SLIDER_THEME_FLAGS_THUMB_AT_BOTTOM_OR_LEFT)
						text_rect.x -= theme.label_theme->font_size_in_pixel + thumb_margin;
					else
						text_rect.x = regioned_track_rect.x + std::max(regioned_track_rect.w, regioned_thumb_rect.w) + thumb_margin;
				}
				immediate_text(text, text_rect, *theme.label_theme);
			}
		}
		return state->flags;
	}
	u16 immediate_slider_float(Rect& track_rect, Rect& thumb_rect, std::string& text, f32 min_value, f32& current_value, f32 max_value, Slider_Theme& theme, u64 hash)
	{
		Slider_State* state = get_or_init(slider_states, hash);
		//assert(max - min >= 0.0f); // start using these
		bool mouse_pressed = Input::IsMouseButtonPressed(MouseCode::Button0);
		auto window_heigth = main_application->GetWindow().GetHeight();
		auto [mouse_x, mouse_y] = Input::get_mouse_pos();
		mouse_y = window_heigth - mouse_y;

		Rect regioned_track_rect = track_rect;
		Rect regioned_thumb_rect = thumb_rect;

		{
			Button_Theme& track_theme = *theme.track_theme;
			v3i p0i{ regioned_track_rect.x,                         regioned_track_rect.y,                         regioned_track_rect.z };
			v3i p1i{ regioned_track_rect.x + regioned_track_rect.w, regioned_track_rect.y,                         regioned_track_rect.z };
			v3i p2i{ regioned_track_rect.x + regioned_track_rect.w, regioned_track_rect.y + regioned_track_rect.h, regioned_track_rect.z };
			v3i p3i{ regioned_track_rect.x,                         regioned_track_rect.y + regioned_track_rect.h, regioned_track_rect.z };
			v4 color = track_theme.background_color;
			immediate_quad(p0i, p1i, p2i, p3i, color);
		}
		{
			Button_Theme& thumb_theme = *theme.thumb_theme;
			regioned_thumb_rect.z = regioned_track_rect.z + 1;

			f32 denom = max_value - min_value;
			if (denom <= 0.0f) denom = 1.0f;
			f32 ratio = (current_value - min_value) / denom;
			if (theme.flags & SLIDER_THEME_FLAGS_IS_HORIZONTAL)
			{
				s32 min_x = regioned_track_rect.x - regioned_thumb_rect.w / 2.0f;
				s32 max_x = regioned_track_rect.x + regioned_track_rect.w - regioned_thumb_rect.w / 2.0f;
				if (theme.flags & SLIDER_THEME_FLAGS_THUMB_IS_INSIDE)
				{
					min_x += regioned_thumb_rect.w / 2.0f;
					max_x -= regioned_thumb_rect.w / 2.0f;
				}
				regioned_thumb_rect.x = Math::lerp(min_x, max_x, ratio);
			}
			else
			{
				s32 min_y = regioned_track_rect.y + regioned_track_rect.h - regioned_thumb_rect.h / 2.0f;
				s32 max_y = regioned_track_rect.y - regioned_thumb_rect.h / 2.0f;
				if (theme.flags & SLIDER_THEME_FLAGS_THUMB_IS_INSIDE)
				{
					min_y -= regioned_thumb_rect.h / 2.0f;
					max_y += regioned_thumb_rect.h / 2.0f;
				}
				regioned_thumb_rect.y = Math::lerp(min_y, max_y, ratio);
			}

			if (theme.flags & SLIDER_THEME_FLAGS_IS_HORIZONTAL)
			{
				regioned_thumb_rect.y = regioned_track_rect.y;
				if (theme.flags & SLIDER_THEME_FLAGS_THUMB_AT_TOP_OR_RIGHT)
					regioned_thumb_rect.y += regioned_track_rect.h + theme.x_or_y_offset_in_pixels;
				else if (theme.flags & SLIDER_THEME_FLAGS_THUMB_AT_BOTTOM_OR_LEFT)
					regioned_thumb_rect.y -= regioned_thumb_rect.h + theme.x_or_y_offset_in_pixels;
				else
					regioned_thumb_rect.y += regioned_track_rect.h / 2.0f - regioned_thumb_rect.h / 2.0f;
			}
			else
			{
				regioned_thumb_rect.x = regioned_track_rect.x;
				if (theme.flags & SLIDER_THEME_FLAGS_THUMB_AT_TOP_OR_RIGHT)
					regioned_thumb_rect.x += regioned_track_rect.w + theme.x_or_y_offset_in_pixels;
				else if (theme.flags & SLIDER_THEME_FLAGS_THUMB_AT_BOTTOM_OR_LEFT)
					regioned_thumb_rect.x -= regioned_thumb_rect.w + theme.x_or_y_offset_in_pixels;
				else
					regioned_thumb_rect.x += regioned_track_rect.w / 2.0f - regioned_thumb_rect.w / 2.0f;
			}

			v4 color = thumb_theme.background_color;
			if (!global_pressed || (global_pressed && pressed_hash == hash))
			{
				bool mouse_over_track = inside(regioned_track_rect, mouse_x, mouse_y);
				bool mouse_over_thumb = inside(regioned_thumb_rect, mouse_x, mouse_y);

				if (mouse_over_track || mouse_over_thumb)
				{
					state->flags |= SLIDER_STATE_FLAGS_OVER;
					color = thumb_theme.background_color_over;

					if (mouse_pressed)
					{
						global_pressed = true;
						pressed_hash = hash;

						if (state->flags & SLIDER_STATE_FLAGS_THUMB_PRESSED)
						{
							state->flags &= (0xffff ^ SLIDER_STATE_FLAGS_THUMB_PRESSED);
							state->flags |= SLIDER_STATE_FLAGS_THUMB_HOLD;
						}
						else
						{
							if (!(state->flags & SLIDER_STATE_FLAGS_THUMB_HOLD))
							{
								state->flags |= SLIDER_STATE_FLAGS_THUMB_PRESSED;
							}
						}
						color = thumb_theme.background_color_pressed;
					}
					else
					{
						global_pressed = false;
						if (state->flags & SLIDER_STATE_FLAGS_THUMB_PRESSED || state->flags & SLIDER_STATE_FLAGS_THUMB_HOLD)
						{
							state->flags &= (0xffff ^ SLIDER_STATE_FLAGS_THUMB_PRESSED);
							state->flags &= (0xffff ^ SLIDER_STATE_FLAGS_THUMB_HOLD);
							state->flags |= SLIDER_STATE_FLAGS_THUMB_RELEASED;
						}
						else
						{
							state->flags &= (0xffff ^ SLIDER_STATE_FLAGS_THUMB_RELEASED);
						}
					}
				}
				else
				{
					state->flags &= (0xffff ^ SLIDER_STATE_FLAGS_OVER);

					if (!mouse_pressed)
					{
						if (state->flags & SLIDER_STATE_FLAGS_THUMB_PRESSED || state->flags & SLIDER_STATE_FLAGS_THUMB_HOLD)
						{
							state->flags &= (0xffff ^ SLIDER_STATE_FLAGS_THUMB_PRESSED);
							state->flags &= (0xffff ^ SLIDER_STATE_FLAGS_THUMB_HOLD);
							state->flags |= SLIDER_STATE_FLAGS_THUMB_RELEASED;
						}
						else
						{
							state->flags &= (0xffff ^ SLIDER_STATE_FLAGS_THUMB_RELEASED);
						}
						global_pressed = false;
						color = thumb_theme.background_color;
					}
					else
					{
						if (state->flags & BUTTON_STATE_FLAGS_PRESSED || state->flags & BUTTON_STATE_FLAGS_HOLD)
							color == thumb_theme.background_color_pressed;
					}
				}
			}

			if ((state->flags & SLIDER_STATE_FLAGS_THUMB_HOLD) || (state->flags & SLIDER_STATE_FLAGS_THUMB_PRESSED))
			{
				if (theme.flags & SLIDER_THEME_FLAGS_IS_HORIZONTAL)
				{
					f32 denom = regioned_track_rect.w;
					if (denom <= 0.0f) denom = 1.0f;
					f32 ratio = ((s32)mouse_x - regioned_track_rect.x) / denom;
					current_value = min_value + ratio * (max_value - min_value);
				}
				else
				{
					f32 denom = regioned_track_rect.h;
					if (denom <= 0.0f) denom = 1.0f;
					f32 ratio = ((regioned_track_rect.y + regioned_track_rect.h) - (s32)mouse_y) / denom;
					current_value = min_value + ratio * (max_value - min_value);
				}
			}

			if (theme.flags & SLIDER_THEME_FLAGS_CLAMP_VALUE)
			{
				current_value = std::max(min_value, current_value);
				current_value = std::min(max_value, current_value);
			}

			v3i p0i{ regioned_thumb_rect.x,                         regioned_thumb_rect.y,                         regioned_thumb_rect.z };
			v3i p1i{ regioned_thumb_rect.x + regioned_thumb_rect.w, regioned_thumb_rect.y,                         regioned_thumb_rect.z };
			v3i p2i{ regioned_thumb_rect.x + regioned_thumb_rect.w, regioned_thumb_rect.y + regioned_thumb_rect.h, regioned_thumb_rect.z };
			v3i p3i{ regioned_thumb_rect.x,                         regioned_thumb_rect.y + regioned_thumb_rect.h, regioned_thumb_rect.z };
			immediate_quad(p0i, p1i, p2i, p3i, color);

			if (theme.flags & SLIDER_THEME_FLAGS_VALUE_SHOWN)
			{
				Rect text_rect = regioned_thumb_rect;
				s32 thumb_margin = 5;

				if (theme.flags & SLIDER_THEME_FLAGS_IS_HORIZONTAL)
				{
					if (theme.flags & SLIDER_THEME_FLAGS_THUMB_AT_TOP_OR_RIGHT)
						text_rect.y += regioned_thumb_rect.h + thumb_margin;
					else if (theme.flags & SLIDER_THEME_FLAGS_THUMB_AT_BOTTOM_OR_LEFT)
						text_rect.y -= theme.label_theme->font_size_in_pixel + thumb_margin;
					else
						text_rect.y = regioned_track_rect.y + std::max(regioned_track_rect.h, regioned_thumb_rect.h) + thumb_margin;
				}
				else
				{
					if (theme.flags & SLIDER_THEME_FLAGS_THUMB_AT_TOP_OR_RIGHT)
						text_rect.x += regioned_thumb_rect.w + thumb_margin;
					else if (theme.flags & SLIDER_THEME_FLAGS_THUMB_AT_BOTTOM_OR_LEFT)
						text_rect.x -= theme.label_theme->font_size_in_pixel + thumb_margin;
					else
						text_rect.x = regioned_track_rect.x + std::max(regioned_track_rect.w, regioned_thumb_rect.w) + thumb_margin;
				}
				immediate_text(text, text_rect, *theme.label_theme);
			}
		}
		return state->flags;
	}

	u16 immediate_text_box(Rect& rect, std::string& text_buffer, u16& cursor, u16& char_count, u16& capacity, bool& global_active, u64& global_active_hash, Text_Box_Theme& theme, u64 hash, bool relative)
	{
		Text_Box_State* state = get_or_init(text_box_states, hash);
		if (state->capacity == 0) state->capacity = capacity;
		if (state->flags & TEXT_BOX_STATE_FLAGS_ACTIVE)
		{
			state->capacity = capacity;
			state->cursor = cursor;
			state->char_count = char_count;
			state->text = text_buffer.substr(0, char_count);
		}

		bool mouse_pressed = Input::IsMouseButtonPressed(MouseCode::Button0);
		auto window_heigth = main_application->GetWindow().GetHeight();
		auto [mouse_x, mouse_y] = Input::get_mouse_pos();
		mouse_y = window_heigth - mouse_y;

		Rect region = rect;
		if (relative)
		{
			update_used_region(rect);
			region = clip_with_regions(rect);
		}
		{
			v3i p0i{ region.x,            region.y,            region.z };
			v3i p1i{ region.x + region.w, region.y,            region.z };
			v3i p2i{ region.x + region.w, region.y + region.h, region.z };
			v3i p3i{ region.x,            region.y + region.h, region.z };
			v4 color = theme.background_color;
			if (state->flags & TEXT_BOX_STATE_FLAGS_ACTIVE)
				color = theme.background_color_active;

			//Delete me later
			{
				bool mouse_over = inside(region, mouse_x, mouse_y);
				if (!mouse_over)
				{
					if (mouse_pressed)
					{
						if (!(state->flags & TEXT_BOX_STATE_FLAGS_PRESSED))
						{
							state->flags &= (0xffff ^ TEXT_BOX_STATE_FLAGS_ACTIVE);
							if (global_active_hash == hash)
								global_active = false;
						}
					}
				}
			}

			if (!global_pressed || (global_pressed && pressed_hash == hash))
			{
				bool mouse_over = inside(region, mouse_x, mouse_y);

				if (mouse_over)
				{
					state->flags |= TEXT_BOX_STATE_FLAGS_OVER;

					if (mouse_pressed)
					{
						global_pressed = true;
						pressed_hash = hash;

						state->flags |= TEXT_BOX_STATE_FLAGS_PRESSED;
					}
					else
					{
						global_pressed = false;
						if (state->flags & TEXT_BOX_STATE_FLAGS_PRESSED)
						{
							state->flags &= (0xffff ^ TEXT_BOX_STATE_FLAGS_PRESSED);
							state->flags |= TEXT_BOX_STATE_FLAGS_RELEASED;
							state->flags |= TEXT_BOX_STATE_FLAGS_ACTIVE;
							{
								global_active = true;
								global_active_hash = hash;
								capacity = state->capacity;
								cursor = state->cursor;
								char_count = state->char_count;
								text_buffer.resize(capacity);
								for (u16 i = 0; i < char_count; i++)
									text_buffer[i] = state->text[i];
							}
						}
						else
						{
							state->flags &= (0xffff ^ TEXT_BOX_STATE_FLAGS_RELEASED);
						}
					}
				}
				else
				{
					state->flags &= (0xffff ^ TEXT_BOX_STATE_FLAGS_OVER);

					if (!mouse_pressed)
					{
						if (state->flags & TEXT_BOX_STATE_FLAGS_PRESSED)
						{
							state->flags &= (0xffff ^ TEXT_BOX_STATE_FLAGS_PRESSED);
							state->flags |= TEXT_BOX_STATE_FLAGS_RELEASED;
						}
						else
						{
							state->flags &= (0xffff ^ TEXT_BOX_STATE_FLAGS_RELEASED);
						}
						global_pressed = false;
					}
					else
					{
						if (!(state->flags & TEXT_BOX_STATE_FLAGS_PRESSED))
						{
							state->flags &= (0xffff ^ TEXT_BOX_STATE_FLAGS_ACTIVE);
							if (global_active_hash == hash)
								global_active = false;
						}
					}
				}
			}
			immediate_quad(p0i, p1i, p2i, p3i, color);
			Rect text_rect = rect;
			text_rect.z++;

			std::string text = "";
			if (state->flags & TEXT_BOX_STATE_FLAGS_ACTIVE)
			{
				text.append(state->text, 0, state->cursor);
				text.append("|");
				text.append(state->text, state->cursor, state->char_count - state->cursor);
			}
			else
			{
				if (state->char_count == 0)
					text.append("Game Name");
				else
					text.append(state->text, 0, state->char_count);
			}

			immediate_text(text, text_rect, *theme.label_theme);
		}

		return state->flags;
	}

	u16 immediate_check_box(Rect& rect, bool& checked, Check_Box_Theme& theme, u64 hash, bool relative)
	{
		Check_Box_State* state = get_or_init(check_box_states, hash);

		bool mouse_pressed = Input::IsMouseButtonPressed(MouseCode::Button0);
		auto window_heigth = main_application->GetWindow().GetHeight();
		auto [mouse_x, mouse_y] = Input::get_mouse_pos();
		mouse_y = window_heigth - mouse_y;
		Rect region = rect;
		if (relative)
		{
			update_used_region(rect);
			region = clip_with_regions(rect);
		}
		{
			v3i p0i{ region.x,            region.y,            region.z };
			v3i p1i{ region.x + region.w, region.y,            region.z };
			v3i p2i{ region.x + region.w, region.y + region.h, region.z };
			v3i p3i{ region.x,            region.y + region.h, region.z };
			v4 color = theme.background_color;

			state->flags &= (0xffff ^ CHECK_BOX_STATE_FLAGS_VALUE_CHANGED);
			if (!global_pressed || (global_pressed && pressed_hash == hash))
			{
				bool mouse_over = inside(region, mouse_x, mouse_y);
				if (mouse_over)
				{
					state->flags |= CHECK_BOX_STATE_FLAGS_OVER;
					color = theme.background_color_over;

					if (mouse_pressed)
					{
						global_pressed = true;
						pressed_hash = hash;

						if (state->flags & CHECK_BOX_STATE_FLAGS_PRESSED)
						{
							state->flags &= (0xffff ^ CHECK_BOX_STATE_FLAGS_PRESSED);
							state->flags |= CHECK_BOX_STATE_FLAGS_HOLD;
						}
						else
						{
							if (!(state->flags & CHECK_BOX_STATE_FLAGS_HOLD))
							{
								state->flags |= CHECK_BOX_STATE_FLAGS_PRESSED;
							}
						}
						color = theme.background_color_pressed;
					}
					else
					{
						global_pressed = false;
						if (state->flags & CHECK_BOX_STATE_FLAGS_PRESSED || state->flags & CHECK_BOX_STATE_FLAGS_HOLD)
						{
							state->flags &= (0xffff ^ CHECK_BOX_STATE_FLAGS_PRESSED);
							state->flags &= (0xffff ^ CHECK_BOX_STATE_FLAGS_HOLD);
							state->flags |= CHECK_BOX_STATE_FLAGS_RELEASED;
							state->flags |= CHECK_BOX_STATE_FLAGS_VALUE_CHANGED;
							checked = !checked;
						}
						else
						{
							state->flags &= (0xffff ^ CHECK_BOX_STATE_FLAGS_RELEASED);
						}
					}
				}
				else
				{
					state->flags &= (0xffff ^ CHECK_BOX_STATE_FLAGS_OVER);

					if (!mouse_pressed)
					{
						if (state->flags & CHECK_BOX_STATE_FLAGS_PRESSED || state->flags & CHECK_BOX_STATE_FLAGS_HOLD)
						{
							state->flags &= (0xffff ^ CHECK_BOX_STATE_FLAGS_PRESSED);
							state->flags &= (0xffff ^ CHECK_BOX_STATE_FLAGS_HOLD);
							state->flags |= CHECK_BOX_STATE_FLAGS_RELEASED;
						}
						else
						{
							state->flags &= (0xffff ^ CHECK_BOX_STATE_FLAGS_RELEASED);
						}
						global_pressed = false;
						color = theme.background_color;
					}
					else
					{
						if (state->flags & CHECK_BOX_STATE_FLAGS_PRESSED || state->flags & CHECK_BOX_STATE_FLAGS_HOLD)
							color == theme.background_color_pressed;
					}
				}
			}
			immediate_quad(p0i, p1i, p2i, p3i, color);

			Rect inside_region = rect;
			inside_region.x += theme.padding;
			inside_region.y += theme.padding;
			inside_region.z += 1;
			inside_region.w -= 2 * theme.padding;
			inside_region.h -= 2 * theme.padding;
			if (relative)
			{
				update_used_region(inside_region);
				inside_region = clip_with_regions(inside_region);
			}
			if (checked)
				immediate_quad(inside_region, theme.background_color_checked);
			else
				immediate_quad(inside_region, theme.background_color_unchecked);
		}
		return state->flags;
	}

	u16 immediate_begin_sub_region(Rect& rect, Sub_Region_Theme& theme, u64 hash)
	{
		Sub_Region_State* state = get_or_init(sub_region_states, hash);

		std::vector<Buffer_Data::Sub_Region>& regions = buffer_data.sub_regions;
		u64 region_count = regions.size();
		buffer_data.sub_region_count++;
		if (buffer_data.sub_region_count > region_count)
		{
			regions.push_back({ rect, rect, rect, &theme, hash });
			region_count++;
		}
		else
		{
			if (hash == regions[region_count - 1].hash)
				regions[region_count - 1].used_region = rect;
			else
				regions[region_count - 1] = { rect, rect, rect, &theme, hash };
		}

		Rect region = regions[0].region;
		for (u64 i = 1; i < region_count; i++)
		{
			Rect& reg = regions[i].region;

			region.x += reg.x;
			region.y += reg.y;
			region.z += reg.z;
			s32 xlim = std::min(reg.x + reg.w, region.x + region.w);
			s32 ylim = std::min(reg.y + reg.h, region.y + region.h);
			s32 w = xlim - region.x;
			s32 h = ylim - region.y;
			if (w <= 0 || h <= 0)
			{
				state->flags |= SUB_REGION_STATE_FLAGS_FULLY_HIDDEN;
				return state->flags;
			}
			region.w = w;
			region.h = h;

		}
		//bool mouse_pressed = Input::IsMouseButtonPressed(MouseCode::Button0);
		auto window_heigth = main_application->GetWindow().GetHeight();
		auto [mouse_x, mouse_y] = Input::get_mouse_pos();
		mouse_y = window_heigth - mouse_y;
		{
			v3i p0i{ region.x,            region.y,            region.z };
			v3i p1i{ region.x + region.w, region.y,            region.z };
			v3i p2i{ region.x + region.w, region.y + region.h, region.z };
			v3i p3i{ region.x,            region.y + region.h, region.z };
			v4 color = theme.background_color;
			immediate_quad(p0i, p1i, p2i, p3i, color);
		}
		if (!global_pressed || (global_pressed && pressed_hash == hash))
		{
			bool mouse_over = inside(region, mouse_x, mouse_y);

			if (mouse_over) state->flags |= BUTTON_STATE_FLAGS_OVER;
			else            state->flags &= (0xffff ^ SUB_REGION_STATE_FLAGS_OVER);
		}


		return state->flags;
	}
	u16 immediate_end_sub_region(s32 track_width)
	{
		assert(buffer_data.sub_region_count > 0);

		std::vector<Buffer_Data::Sub_Region>& regions = buffer_data.sub_regions;
		auto& the_most_inner_region = regions[buffer_data.sub_region_count - 1];
		buffer_data.sub_region_count--;

		Sub_Region_State* state = get_or_init(sub_region_states, the_most_inner_region.hash);

		{
			Rect& region = the_most_inner_region.region;
			Rect& used_region = the_most_inner_region.used_region;
			the_most_inner_region.prev_frame_used_region = used_region;
			Sub_Region_Theme& theme = *the_most_inner_region.theme;

			std::string text = "";

			bool left_most = region.x > used_region.x;
			bool right_most = region.x + region.w < used_region.x + used_region.w;
			if (theme.flags & SUB_REGION_THEME_FLAGS_HORIZONTALLY_SCROLLABLE && (left_most || right_most))
			{
				Rect track_rect;
				track_rect.x = region.x;
				track_rect.y = region.y - track_width;
				track_rect.z = region.z + 1;
				track_rect.w = region.w;
				track_rect.h = track_width;

				Rect thumb_rect;
				thumb_rect.w = track_width - 2;
				thumb_rect.h = track_width - 2;

				u64 horizontal_scroll_bar_hash = ((u64)1 << 32) + the_most_inner_region.hash;
				immediate_slider_float(track_rect, thumb_rect, text, 0, state->horizontal_scroll_percentage, 1.0f, *theme.horizontal_slider_theme, horizontal_scroll_bar_hash);
			}

			bool bottom_most = region.y > used_region.y;
			bool top_most = region.y + region.h < used_region.y + used_region.h;
			if (theme.flags & SUB_REGION_THEME_FLAGS_VERTICALLY_SCROLLABLE && (bottom_most || top_most))
			{
				Rect track_rect;
				track_rect.x = region.x + region.w;
				track_rect.y = region.y;
				track_rect.z = region.z + 1;
				track_rect.w = track_width;
				track_rect.h = region.h;

				Rect thumb_rect;
				thumb_rect.w = track_width - 2;

				assert(the_most_inner_region.region.h > 0);
				assert(the_most_inner_region.used_region.h > 0);

				thumb_rect.h = (track_rect.h) / 2 * ((f32)the_most_inner_region.region.h / the_most_inner_region.used_region.h);

				u64 vertical_scroll_bar_hash = ((u64)2 << 32) + the_most_inner_region.hash;
				immediate_slider_float(track_rect, thumb_rect, text, 0, state->vertical_scroll_percentage, 1.0f, *theme.vertical_slider_theme, vertical_scroll_bar_hash);
			}
		}

		return state->flags;
	}

	void immediate_flush()
	{
		buffer_data.vertex_buffer->SetData((f32*)buffer_data.buffer_base, buffer_data.buffer_cursor * sizeof(Vertex));
		buffer_data.vertex_array->Bind();

		for (size_t i = 0; i < buffer_data.texture_slots_cursor; i++)
			buffer_data.texture_slots[i]->Bind(i);
		RenderCommand::DrawIndexed(buffer_data.vertex_array, buffer_data.quad_count);

		buffer_data.texture_slots_cursor = 1;
		buffer_data.quad_count = 0;
		buffer_data.buffer_cursor = 0;
	}
	void* load_font(const std::string& path)
	{
		return new Font(path);
	}
}