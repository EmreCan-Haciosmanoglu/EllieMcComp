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

	void immediate_quad(
		v2& p0, v2& p1, v2& p2, v2& p3,
		v2& uv0, v2& uv1, v2& uv2, v2& uv3,
		v4& color);
	void immediate_quad(
		v2i& p0i, v2i& p1i, v2i& p2i, v2i& p3i,
		v2& uv0, v2& uv1, v2& uv2, v2& uv3,
		v4& color, f32 texture_index);
	void immediate_quad(
		v2& p0, v2& p1, v2& p2, v2& p3,
		v2& uv0, v2& uv1, v2& uv2, v2& uv3,
		v4& color, f32 texture_index);
	void immediate_quad(
		v2& p0,  v2& p1,  v2& p2,  v2& p3,   
		v4& color);
	void immediate_quad(
		v2i& p0i, v2i& p1i, v2i& p2i, v2i& p3i, 
		v4& color);
	void immediate_quad(Rect& r, v4& color);

	void immediate_text(std::string& text, Rect& r, Label_Theme& theme);

	u8 immediate_button(Rect& r, std::string& text, Button_Theme& theme, u64 hash);

	void immediate_flush();

}