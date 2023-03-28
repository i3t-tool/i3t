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

in vec3 nearPoint;
in vec3 farPoint;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

uniform float u_near;
uniform float u_far;

float gridSize = 1.0f;
float axisWidth = 1.0f;

vec4 grid(vec3 worldPos) {
	vec2 coord = worldPos.xz * gridSize;
	vec2 derivative = fwidth(coord);
	vec2 grid = abs(fract(coord - 0.5) - 0.5) / derivative;
	float line = min(grid.x, grid.y);
	float minimumz = min(derivative.y, 1);
	float minimumx = min(derivative.x, 1);
	vec4 color = vec4(0.45, 0.49, 0.53, 1.0 - min(line, 1.0));

	// z axis
	if (worldPos.x > -axisWidth * minimumx && worldPos.x < axisWidth * minimumx)
	color.z = 1.0;
	// x axis
	if (worldPos.z > -axisWidth * minimumz && worldPos.z < axisWidth * minimumz)
	color.x = 1.0;
	return color;
}

float linearize_depth(float d, float zNear, float zFar)
{
	float z_ndc = 2.0 * d - 1.0;
	return 2.0 * zNear * zFar / (zFar + zNear - z_ndc * (zFar - zNear));
}

void main()
{
	float t = -nearPoint.y / (farPoint.y - nearPoint.y);
	vec3 worldPos = nearPoint + t * (farPoint - nearPoint);

	// Compute depth from world position
	vec4 clipSpacePos = projectionMatrix * viewMatrix * vec4(worldPos.xyz, 1.0);
	float clipSpaceDepth = (clipSpacePos.z / clipSpacePos.w);
	gl_FragDepth = 0.5 + 0.5 * clipSpaceDepth;

	float linearDepth = linearize_depth(clipSpaceDepth, u_near, u_far);
	float linearDepthNormalized = linearDepth / u_far;
	float fading = max(0, (0.5 - linearDepthNormalized));

	FragColor = grid(worldPos) * float(t > 0);
	FragColor.a *= fading;

	// WBOIT
	if (u_wboitFlag) {
		////////     GRID SHADER     /////////
		float fragCoordZ = gl_FragDepth;
		float wboitDepth = 1.0;
		if (u_wboitFunc != 0) {
			wboitDepth = linearDepth;
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
