#version 330 core

out vec4 FragColor;

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

void main()
{
	float t = -nearPoint.y / (farPoint.y - nearPoint.y);
	vec3 worldPos = nearPoint + t * (farPoint - nearPoint);

	// Compute depth from world position
	vec4 clipSpacePos = projectionMatrix * viewMatrix * vec4(worldPos.xyz, 1.0);
	float clipSpaceDepth = (clipSpacePos.z / clipSpacePos.w);
	gl_FragDepth = 0.5 + 0.5 * clipSpaceDepth;

	float linearDepth = (2.0 * near * far) / (far + near - clipSpaceDepth * (far - near));
	float linearDepthNormalized = linearDepth / far;
	float fading = max(0, (0.5 - linearDepthNormalized));

	FragColor = grid(worldPos) * float(t > 0);
	FragColor.a *= fading;
}
