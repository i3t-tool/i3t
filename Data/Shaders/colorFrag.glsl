#version 330 core

layout (location = 0) out vec4 FragColor;

// WBOIT
uniform bool u_wboitFlag = false;
layout (location = 1) out vec4 AccumulationBuffer;
layout (location = 2) out float RevealageBuffer;
// END WBOIT

in vec3 Color;

uniform float u_opacity;

void main()
{
	FragColor = vec4(Color, u_opacity);

	// WBOIT
	if (u_wboitFlag) {
		float weight = FragColor.a * max(1e-2, min(3e3, 0.03/(1e-5 + pow(gl_FragCoord.z/200, 4))));
//		float weight = clamp(pow(min(1.0, FragColor.a * 10.0) + 0.01, 3.0) * 1e8 *
//		pow(1.0 - gl_FragCoord.z * 0.9, 3.0), 1e-2, 3e3);

		// Store accumulation color (RGB) and revealage factor (A)
		AccumulationBuffer = vec4(FragColor.rgb * FragColor.a * weight, FragColor.a);
		// Store accumulation alpha (R)
		RevealageBuffer = FragColor.a * weight;
	}
	// END WBOIT
}