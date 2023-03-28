#version 330 core

layout (location = 0) out vec4 FragColor;

// WBOIT
uniform bool u_wboitFlag = false;
uniform int u_wboitFunc = 0;
uniform float u_wboitNear = 0.1;
uniform float u_wboitFar = 500;
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
		//////// Customize if needed /////////
		float fragCoordZ = gl_FragCoord.z;
		float wboitDepth = 1.0;
		if (u_wboitFunc != 0) {
			wboitDepth = 2.0 * u_wboitNear * u_wboitFar / (u_wboitFar + u_wboitNear - (2.0 * gl_FragCoord.z - 1.0) * (u_wboitFar - u_wboitNear));
		}
		//////////////////////////////////////
		// Branching based on which weight function is selected
		float weight = 1.0;
		switch (u_wboitFunc) {
			case 0:// Off
			weight = 1.0;
			break;
			case 1:// Wboit paper Eq. 7
			weight = FragColor.a * max(1e-2, min(3e3, 10/(1e-5 + pow(wboitDepth/5, 2) + pow(wboitDepth/200, 6))));
			break;
			case 2:// Wboit paper Eq. 8
			weight = FragColor.a * max(1e-2, min(3e3, 10/(1e-5 + pow(wboitDepth/10, 3) + pow(wboitDepth/200, 6))));
			break;
			case 3:// Wboit paper Eq. 9
			weight = FragColor.a * max(1e-2, min(3e3, 0.03/(1e-5 + pow(wboitDepth/200, 4))));
			break;
			case 4:// Wboit paper Eq. 10
			weight = FragColor.a * max(1e-2, 3e3 * pow(1 - fragCoordZ, 3));
			break;
			case 5:// LearnOpengl example 1 (Eq. 9 with color bias)
			weight = max(min(1.0, max(max(FragColor.r, FragColor.g), FragColor.b) * FragColor.a), FragColor.a) * clamp(0.03 / (1e-5 + pow(wboitDepth / 200, 4.0)), 1e-2, 3e3);
			break;
			case 6:// LearnOpengl example 2 (Eq. 10 with changes)
			weight = clamp(pow(min(1.0, FragColor.a * 10.0) + 0.01, 3.0) * 1e8 * pow(1.0 - fragCoordZ * 0.9, 3.0), 1e-2, 3e3);
			break;
			case 7:// z^-3
			weight = pow(wboitDepth, -3);
			break;
			case 8:// abs(z - zFar + Eps)
			weight = abs(wboitDepth - u_wboitFar + 0.00001);
			break;
		}
		// Store accumulation color (RGB) and revealage factor (A)
		AccumulationBuffer = vec4(FragColor.rgb * FragColor.a * weight, FragColor.a);
		// Store accumulation alpha (R)
		RevealageBuffer = FragColor.a * weight;
	}
	// END WBOIT
}