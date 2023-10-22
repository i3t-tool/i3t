#version 330 core

layout (location = 0) out vec4 FragColor;

uniform sampler2D source;

void main()
{
	ivec2 coords = ivec2(gl_FragCoord.xy);
	FragColor = vec4(texelFetch(source, coords, 0));
}