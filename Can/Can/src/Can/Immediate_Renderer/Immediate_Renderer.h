#pragma once

#include "Can/Renderer/RenderCommand.h"
#include "Can/Renderer/Shader.h"
#include "Can/Renderer/Texture.h"

#include "Can/Camera/Orthographic_Camera_Controller.h"

#include "Can/Immediate_Renderer/Widgets.h"
#include "Can/Immediate_Renderer/Themes.h"
#include "Can/Immediate_Renderer/States.h"

namespace Can
{

	void init_immediate_renderer();

	void set_camera_for_immediate_renderer();

	void immediate_quad(const v2& p0, const v2& p1, const v2& p2, const v2& p3, const v2& uv0, const v2& uv1, const v2& uv2, const v2& uv3, const v4& color);
	void immediate_quad(const v2i& p0i, const v2i& p1i, const v2i& p2i, const v2i& p3i, const v2& uv0, const v2& uv1, const v2& uv2, const v2& uv3, const v4& color, f32 texture_index);
	void immediate_quad(const v3i& p0i, const v3i& p1i, const v3i& p2i, const v3i& p3i, const v2& uv0, const v2& uv1, const v2& uv2, const v2& uv3, const v4& color, f32 texture_index);
	void immediate_quad(const v2& p0, const v2& p1, const v2& p2, const v2& p3, const v2& uv0, const v2& uv1, const v2& uv2, const v2& uv3, const v4& color, f32 texture_index);
	void immediate_quad(const v3& p0, const v3& p1, const v3& p2, const v3& p3, const v2& uv0, const v2& uv1, const v2& uv2, const v2& uv3, const v4& color, f32 texture_index);
	void immediate_quad(const v2& p0, const v2& p1, const v2& p2, const v2& p3, const v4& color);
	void immediate_quad(const v3& p0, const v3& p1, const v3& p2, const v3& p3, const v4& color);
	void immediate_quad(const v2i& p0i, const v2i& p1i, const v2i& p2i, const v2i& p3i, const v4& color);
	void immediate_quad(const v3i& p0i, const v3i& p1i, const v3i& p2i, const v3i& p3i, const v4& color);

	void immediate_quad(const Rect& rect, const v4& color, bool relative = false);
	void immediate_image(const Rect& rect, const Ref<Texture2D>& image, bool relative = false);
	void immediate_tinted_image(const Rect& rect, const Ref<Texture2D>& image, const v4& tint, bool relative = false);

	void immediate_text(const std::string& text, Rect& rect, Label_Theme& theme, bool relative = true);

	u16 immediate_button(Rect& rect, const std::string& text, Button_Theme& theme, u64 hash, bool relative = true);
	u16 immediate_image_button(Rect& rect, Button_Theme& theme, Ref<Texture2D> texture, u64 hash, bool relative = true);
	u16 immediate_drop_down_list(Rect& rect, std::vector<std::string>& list, u64& selected_item, Drop_Down_List_Theme& theme, u64 hash);
	u16 immediate_slider(Rect& track_rect, Rect& thumb_rect, const std::string& text, s16 min_value, s16& current_value, s16 max_value, Slider_Theme& theme, u64 hash);
	u16 immediate_slider_float(Rect& track_rect, Rect& thumb_rect, const std::string& text, f32 min_value, f32& current_value, f32 max_value, Slider_Theme& theme, u64 hash);
	u16 immediate_text_box(Rect& rect, std::string& text_buffer, u16& cursor, u16& char_count, u16& capacity, bool& global_active, u64& global_active_hash, Text_Box_Theme& theme, u64 hash, bool relative = true);
	u16 immediate_check_box(Rect& rect, bool& checked, Check_Box_Theme& theme, u64 hash, bool relative = true);
	u16 immediate_begin_sub_region(Rect& rect, Sub_Region_Theme& theme, u64 hash);
	u16 immediate_end_sub_region(s32 track_width);

	void immediate_flush();
	void* load_font(const std::string& path);
}