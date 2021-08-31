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

	void immediate_quad(v2& p0, v2& p1, v2& p2, v2& p3, v2& uv0, v2& uv1, v2& uv2, v2& uv3, v4& color);
	void immediate_quad(v2i& p0i, v2i& p1i, v2i& p2i, v2i& p3i, v2& uv0, v2& uv1, v2& uv2, v2& uv3, v4& color, f32 texture_index);
	void immediate_quad(v3i& p0i, v3i& p1i, v3i& p2i, v3i& p3i, v2& uv0, v2& uv1, v2& uv2, v2& uv3, v4& color, f32 texture_index);
	void immediate_quad(v2& p0, v2& p1, v2& p2, v2& p3, v2& uv0, v2& uv1, v2& uv2, v2& uv3, v4& color, f32 texture_index);
	void immediate_quad(v3& p0, v3& p1, v3& p2, v3& p3, v2& uv0, v2& uv1, v2& uv2, v2& uv3, v4& color, f32 texture_index);
	void immediate_quad(v2& p0, v2& p1, v2& p2, v2& p3, v4& color);
	void immediate_quad(v3& p0, v3& p1, v3& p2, v3& p3, v4& color);
	void immediate_quad(v2i& p0i, v2i& p1i, v2i& p2i, v2i& p3i, v4& color);
	void immediate_quad(v3i& p0i, v3i& p1i, v3i& p2i, v3i& p3i, v4& color);
	void immediate_quad(Rect& r, v4& color);

	void immediate_text(const std::string& text, Rect& r, Label_Theme& theme);

	u16 immediate_button(Rect r, const std::string& text, Button_Theme& theme, u64 hash);
	u16 immediate_drop_down_list(Rect& r, std::vector<std::string>& list, u64& selected_item, Drop_Down_List_Theme& theme, u64 hash);
	u16 immediate_slider_float(Rect& track_rect, Rect thumb_rect, std::string& text, f32 min_value, f32& current_value, f32 max_value, Slider_Theme& theme, u64 hash);

	void immediate_flush();

	void* load_font(const std::string& path);

}