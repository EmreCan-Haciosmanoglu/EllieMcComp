#include "canpch.h"

#include "Immediate_Renderer.h"

#include "Can/EntryPoint.h"


namespace Can
{
	Buffer_Data buffer_data;

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

		//buffer_data.font = new Font("assets/fonts/Poppins/Poppins-Medium.ttf");
		//buffer_data.font_atlas.push_back(FontAtlas(buffer_data.font->GetFace(), 24));
		delete[] indices;
	}

	void set_camera_for_immediate_renderer()
	{
		auto& camera = buffer_data.camera_controller->m_Camera;
		buffer_data.shader->Bind();
		buffer_data.shader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
	}

	void immediate_quad(Rect& r, v4& color)
	{
		auto& window = main_application->GetWindow();

		u64 width_in_pixels = window.GetWidth();
		u64 height_in_pixels = window.GetHeight();

		f32 aspect_ratio = buffer_data.camera_controller->m_AspectRatio;
		f32 zoom_level = buffer_data.camera_controller->m_ZoomLevel;

		f32 width_float = 2.0f * aspect_ratio * zoom_level;
		f32 height_float = 2.0f * zoom_level;

		Vertex vertex;
		vertex.position = {
			((r.x / width_in_pixels) - 0.5f) * width_float,
			((r.y / height_in_pixels) - 0.5f) * height_float,
			0.0f
		};
		vertex.uv = { 0.0f, 0.0f };
		vertex.tint_color = color;
		vertex.texture_index = 0.0f;
		vertex.tiling_factor = 1.0f;

		buffer_data.buffer_base[buffer_data.buffer_cursor] = vertex;
		buffer_data.buffer_cursor++;

		vertex.position = {
			(((r.x + r.w) / width_in_pixels) - 0.5f) * width_float,
			((r.y / height_in_pixels) - 0.5f) * height_float,
			0.0f
		};
		vertex.uv = { 1.0f, 0.0f };
		buffer_data.buffer_base[buffer_data.buffer_cursor] = vertex;
		buffer_data.buffer_cursor++;

		vertex.position = {
			(((r.x + r.w) / width_in_pixels) - 0.5f) * width_float,
			(((r.y + r.h) / height_in_pixels) - 0.5f) * height_float,
			0.0f
		};
		vertex.uv = { 1.0f, 1.0f };
		buffer_data.buffer_base[buffer_data.buffer_cursor] = vertex;
		buffer_data.buffer_cursor++;

		vertex.position = {
			((r.x / width_in_pixels) - 0.5f) * width_float,
			(((r.y + r.h) / height_in_pixels) - 0.5f) * height_float,
			0.0f
		};
		vertex.uv = { 0.0f, 1.0f };
		buffer_data.buffer_base[buffer_data.buffer_cursor] = vertex;
		buffer_data.buffer_cursor++;


		buffer_data.quad_count += 6;
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
}