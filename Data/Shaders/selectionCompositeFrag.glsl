#version 330 core

layout (location = 0) out vec4 FragColor;

uniform sampler2D source;

uniform vec2 u_resolution;
uniform float u_cutoff;

void main()
{
	vec2 uv = gl_FragCoord.xy / u_resolution.xy;
	vec4 color = texture(source, uv);

	//FragColor = vec4(texture(source, uv));
	//FragColor = vec4(texture(source, uv).xyz, 1.0);
	//FragColor = vec4(1.0, 0.0, 0.0, 0.5);
	//FragColor = vec4(smoothstep(0, 1, texture(source, uv)));
	//FragColor = vec4(step(0.1, texture(source, uv)));
	FragColor = vec4(color.rgb, smoothstep(0.0, u_cutoff, color.a));
}