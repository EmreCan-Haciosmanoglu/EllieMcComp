#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_TintColor;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in float a_TextureID;

uniform mat4 u_ViewProjection;

out vec4 v_TintColor;
out vec2 v_TexCoord;
out float v_TextureID;

void main()
{
	gl_Position = u_ViewProjection * vec4(a_Position, 1.0);

	v_TintColor = a_TintColor;
	v_TexCoord = a_TexCoord;
	v_TextureID = a_TextureID;
}

#type fragment
#version 330 core

layout(location = 0) out vec4 color;

uniform float u_TilingFactor;
uniform sampler2D u_Texture;

in vec4 v_TintColor;
in vec2 v_TexCoord;
in float v_TextureID;

void main()
{
	int texID = int(v_TextureID);
	//color = texture(texID, v_TexCoord * u_TilingFactor) * v_TintColor;
	color = v_TintColor;
}