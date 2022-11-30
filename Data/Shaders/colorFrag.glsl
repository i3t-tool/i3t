#version 330 core

out vec4 FragColor;

in vec3 Color;

uniform float u_opacity;

void main()
{
	FragColor = vec4(Color, u_opacity);
}