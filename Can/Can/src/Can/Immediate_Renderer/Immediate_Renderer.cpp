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
#define SOME_DENOM 100.0f

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
	void immediate_quad(Rect& r, v4& color)
	{
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
	void immediate_text(const std::string& text, Rect& r, Label_Theme& theme)
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
		for (const s8* p = text.c_str(); *p; ++p)
			text_width += chars[*p].advanceX;


		s32 atlas_width = atlas->width;
		s32 atlas_height = atlas->height;

		v3i pos{ r.x + r.w / 2 - text_width / 2, r.y + r.h / 2 - atlas_height / 2, r.z };

		if (theme.flags & FontFlags::LeftAligned)
		{
			pos.x = r.x;
		}
		else if (theme.flags & FontFlags::RightAligned)
		{
			pos.x = r.x + r.w - text_width;
		}

		if (theme.flags & FontFlags::TopAligned)
		{
			pos.y = r.y + r.h - atlas_height;
		}
		else if (theme.flags & FontFlags::BottomAligned)
		{
			pos.y = r.y;
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

		for (const char* p = text.c_str(); *p; ++p)
		{
			s32 x = pos.x + chars[*p].bitmapLeft;
			s32 y = pos.y + chars[*p].bitmapTop;
			s32 z = pos.z;
			s32 w = chars[*p].bitmapWidth;
			s32 h = chars[*p].bitmapHeight;

			v3i p0i{ x,     y - h, z };
			v3i p1i{ x + w, y - h, z };
			v3i p2i{ x + w, y,     z };
			v3i p3i{ x,     y,     z };

			v2 uv0{ chars[*p].xOffset, chars[*p].bitmapHeight / atlas_height };
			v2 uv1{ chars[*p].xOffset + chars[*p].bitmapWidth / atlas_width, chars[*p].bitmapHeight / atlas_height };
			v2 uv2{ chars[*p].xOffset + chars[*p].bitmapWidth / atlas_width, 0.0f };
			v2 uv3{ chars[*p].xOffset, 0.0f };

			// Calculate kerning value
			FT_Vector kerning;
			FT_Get_Kerning(
				font->face,	// font face handle
				*p,								// left glyph
				*(p + 1),						// right glyph
				FT_KERNING_DEFAULT,				// kerning mode
				&kerning);						// variable to store kerning value

			pos.x += chars[*p].advanceX + (kerning.x >> 6);
			pos.y += chars[*p].advanceY;
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
	u16 immediate_button(Rect r, const std::string& text, Button_Theme& theme, u64 hash)
	{
		Button_State* state = get_or_init(button_states, hash);

		bool mouse_pressed = Input::IsMouseButtonPressed(MouseCode::Button0);
		auto window_heigth = main_application->GetWindow().GetHeight();
		auto [mouse_x, mouse_y] = Input::get_mouse_pos();
		mouse_y = window_heigth - mouse_y;

		{
			v3i p0i{ r.x,       r.y,       r.z};
			v3i p1i{ r.x + r.w, r.y,       r.z};
			v3i p2i{ r.x + r.w, r.y + r.h, r.z};
			v3i p3i{ r.x,       r.y + r.h, r.z};
			v4 color = theme.background_color;
			if (!global_pressed || (global_pressed && pressed_hash == hash))
			{
				bool mouse_over = inside(r, mouse_x, mouse_y);
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
			r.z++;
			immediate_text(text, r, *theme.label_theme);
		}
		return state->flags;
	}

	u16 immediate_drop_down_list(Rect& r, std::vector<std::string>& list, u64& selected_item, Drop_Down_List_Theme& theme, u64 hash)
	{
		bool first_time = false;
		Drop_Down_List_State* state = get_or_init(drop_down_list_states, hash, first_time);
		if (first_time) state->active_item = selected_item;

		state->flags &= (0xffff ^ DROP_DOWN_LIST_STATE_FLAGS_ITEM_CHANGED);

		bool mouse_pressed = Input::IsMouseButtonPressed(MouseCode::Button0);
		auto window_heigth = main_application->GetWindow().GetHeight();
		auto [mouse_x, mouse_y] = Input::get_mouse_pos();
		mouse_y = window_heigth - mouse_y;
		bool over_children = false;
		{
			if (state->flags & DROP_DOWN_LIST_STATE_FLAGS_OPEN)
			{
				Rect lr = r;
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
			u16 flags = immediate_button(r, list[state->active_item], *theme.button_theme, hash);
			if (flags & BUTTON_STATE_FLAGS_RELEASED)
				state->flags ^= DROP_DOWN_LIST_STATE_FLAGS_OPEN;
			state->flags &= 0xfff0;
			state->flags |= flags;
		}
		return state->flags;
	}

	u16 immediate_slider_float(Rect& track_rect, Rect thumb_rect, std::string& text, f32 min_value, f32& current_value, f32 max_value, Slider_Theme& theme, u64 hash)
	{
		Slider_State* state = get_or_init(slider_states, hash);
		//assert(max - min >= 0.0f); // start using these
		bool mouse_pressed = Input::IsMouseButtonPressed(MouseCode::Button0);
		auto window_heigth = main_application->GetWindow().GetHeight();
		auto [mouse_x, mouse_y] = Input::get_mouse_pos();
		mouse_y = window_heigth - mouse_y;
		{
			Button_Theme& track_theme = *theme.track_theme;
			v3i p0i{ track_rect.x,                track_rect.y,                track_rect.z };
			v3i p1i{ track_rect.x + track_rect.w, track_rect.y,                track_rect.z };
			v3i p2i{ track_rect.x + track_rect.w, track_rect.y + track_rect.h, track_rect.z };
			v3i p3i{ track_rect.x,                track_rect.y + track_rect.h, track_rect.z };
			v4 color = track_theme.background_color;
			immediate_quad(p0i, p1i, p2i, p3i, color);
		}
		{
			Button_Theme& thumb_theme = *theme.thumb_theme;
			thumb_rect.z = track_rect.z + 1;

			f32 denom = max_value - min_value;
			if (!denom) denom = 1.0f;
			f32 ratio = (current_value - min_value) / denom;
			thumb_rect.x = track_rect.x + Math::lerp(0.0f, track_rect.w, ratio) - thumb_rect.w / 2.0f;

			thumb_rect.y = track_rect.y;
			if (theme.flags & SLIDER_THEME_FLAGS_THUMB_AT_TOP)
				thumb_rect.y += track_rect.h + theme.y_offset_in_pixels;
			else if (theme.flags & SLIDER_THEME_FLAGS_THUMB_AT_BOTTOM)
				thumb_rect.y -= thumb_rect.h + theme.y_offset_in_pixels;
			else
				thumb_rect.y += track_rect.h / 2.0f - thumb_rect.h / 2.0f;

			v4 color = thumb_theme.background_color;
			if (!global_pressed || (global_pressed && pressed_hash == hash))
			{
				bool mouse_over_track = inside(track_rect, mouse_x, mouse_y);
				bool mouse_over_thumb = inside(thumb_rect, mouse_x, mouse_y);

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
				f32 denom = track_rect.w;
				if (!denom) denom = 1.0f;
				f32 ratio = ((s32)mouse_x - track_rect.x) / denom;
				current_value = min_value + ratio * (max_value - min_value);
			}

			if (theme.flags & SLIDER_THEME_FLAGS_CLAMP_VALUE)
			{
				current_value = std::max(min_value, current_value);
				current_value = std::min(max_value, current_value);
			}

			v3i p0i{ thumb_rect.x,                thumb_rect.y,                thumb_rect.z };
			v3i p1i{ thumb_rect.x + thumb_rect.w, thumb_rect.y,                thumb_rect.z };
			v3i p2i{ thumb_rect.x + thumb_rect.w, thumb_rect.y + thumb_rect.h, thumb_rect.z };
			v3i p3i{ thumb_rect.x,                thumb_rect.y + thumb_rect.h, thumb_rect.z };
			immediate_quad(p0i, p1i, p2i, p3i, color);

			if (theme.flags & SLIDER_THEME_FLAGS_VALUE_SHOWN)
			{
				Rect text_rect = thumb_rect;
				s32 thumb_margin = 5;

				if (theme.flags & SLIDER_THEME_FLAGS_THUMB_AT_TOP)
					text_rect.y += thumb_rect.h + thumb_margin;
				else if (theme.flags & SLIDER_THEME_FLAGS_THUMB_AT_BOTTOM)
					text_rect.y -= theme.label_theme->font_size_in_pixel + thumb_margin;
				else
					text_rect.y = track_rect.y + std::max(track_rect.h, thumb_rect.h) + thumb_margin;

				immediate_text(text, text_rect, *theme.label_theme);
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