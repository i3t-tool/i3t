#version 330 core

layout (location = 0) out vec4 FragColor;

// WBOIT
uniform bool u_wboitFlag = false;
layout (location = 1) out vec4 AccumulationBuffer;
layout (location = 2) out float RevealageBuffer;
// END WBOIT

in vec3 nearPoint;
in vec3 farPoint;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

uniform float near;
uniform float far;

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
	float depth = 2.0 * zNear * zFar / (zFar + zNear - z_ndc * (zFar - zNear));
	return depth / zFar;// Normalize depth range
}

void main()
{
	float t = -nearPoint.y / (farPoint.y - nearPoint.y);
	vec3 worldPos = nearPoint + t * (farPoint - nearPoint);

	// Compute depth from world position
	vec4 clipSpacePos = projectionMatrix * viewMatrix * vec4(worldPos.xyz, 1.0);
	float clipSpaceDepth = (clipSpacePos.z / clipSpacePos.w);
	gl_FragDepth = 0.5 + 0.5 * clipSpaceDepth;

	float linearDepth = linearize_depth(clipSpaceDepth, near, far);
	float fading = max(0, (0.5 - linearDepth));

	FragColor = grid(worldPos) * float(t > 0);
	FragColor.a *= fading;

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
