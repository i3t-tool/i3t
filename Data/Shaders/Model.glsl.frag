#version 450

layout(location = 3) in vec3 v_Position;
layout(location = 4) in vec2 v_UV;
layout(location = 5) in vec3 v_Normal;

layout(location = 0) out vec4 f_Color;

uniform sampler2D texSampler;

void main()
{
	f_Color = texture(texSampler, v_UV);
}
