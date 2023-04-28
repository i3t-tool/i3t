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

uniform vec3 u_gridColor = vec3(0.45, 0.49, 0.53);

uniform float u_gridSize = 1.0f;
uniform float u_gridStrength = 0.5f;
uniform float u_gridFadeOffset = 0.2f;
uniform float u_axisWidth = 0.9f;

vec3 gridColor;
float gridSize;
float gridStrength;
float gridFadeOffset;
float axisWidth;
float lineWidth = 1.0f;

const int GRID_XY = 1;
const int GRID_XZ = 2;
const int GRID_YZ = 3;

const float ALPHA_CUTOFF = 0.0001f;

float linearize_depth(float d, float zNear, float zFar)
{
	float z_ndc = 2.0 * d - 1.0;
	return 2.0 * zNear * zFar / (zFar + zNear - z_ndc * (zFar - zNear));
}

vec4 axis(vec3 worldPos, int firstAxis, int secondAxis, bool drawFirstAxis, bool drawSecondAxis)
{
	vec2 coord;
	coord.x = worldPos[firstAxis] * gridSize;
	coord.y = worldPos[secondAxis] * gridSize;

	vec2 derivative = fwidth(coord) * lineWidth;
	float size = 1.5f;
	float xAxis = size - clamp(abs(coord.x), 0, size) / min(derivative.x, 0.5f);
	float yAxis = size - clamp(abs(coord.y), 0, size) / min(derivative.y, 0.5f);
	float line = max(xAxis, yAxis);

	vec4 color = vec4(gridColor, line);

	if (xAxis > yAxis) {
		color[secondAxis] = 1.0;
	} else {
		color[firstAxis] = 1.0;
	}
	return color;
}

vec4 drawAxis(int gridType, bool drawFirstAxis, bool drawSecondAxis, bool axesOnly, out float depth, out float linearDepth)
{
	int firstAxis;
	int secondAxis;
	int otherAxis;
	if (gridType == GRID_XY) {
		firstAxis = 0;
		secondAxis = 1;
		otherAxis = 2;
	} else if (gridType == GRID_YZ) {
		firstAxis = 1;
		secondAxis = 2;
		otherAxis = 0;
	} else {
		firstAxis = 0;
		secondAxis = 2;
		otherAxis = 1;
	}

	// Figure out where camera ray intersects world plane
	float t = -nearPoint[otherAxis] / (farPoint[otherAxis] - nearPoint[otherAxis]);
	vec3 worldPos = nearPoint + t * (farPoint - nearPoint);

	// Compute depth from world position
	vec4 clipSpacePos = projectionMatrix * viewMatrix * vec4(worldPos.xyz, 1.0);
	float clipSpaceDepth = (clipSpacePos.z / clipSpacePos.w);
	depth = 0.5 + 0.5 * clipSpaceDepth;

	vec4 color = axis(worldPos, firstAxis, secondAxis, drawFirstAxis, drawSecondAxis) * float(t > 0);

	linearDepth = linearize_depth(clipSpaceDepth, u_near, u_far);
	float linearDepthNormalized = linearDepth / u_far;

	float fading = max(0, (0.5 - linearDepthNormalized));
	fading = smoothstep(1.0f - gridFadeOffset, 0.2f, linearDepthNormalized + gridFadeOffset);
	color.a *= fading * gridStrength;

	return color;
}

vec4 grid(vec3 worldPos, int firstAxis, int secondAxis, bool drawFirstAxis, bool drawSecondAxis, bool axisOnly)
{
	vec2 coord;
	coord.x = worldPos[firstAxis] * gridSize;
	coord.y = worldPos[secondAxis] * gridSize;

	vec2 derivative = fwidth(coord) * lineWidth;
	vec2 grid = abs(fract(coord - 0.5) - 0.5) / derivative;
	float line = min(grid.x, grid.y);
	vec4 color = vec4(gridColor, 1.0 - min(line, 1.0));

	float minimumx = min(derivative.x, 1);
	float minimumy = min(derivative.y, 1);

	bool axisDrawn = false;
	if (drawFirstAxis) {
		if (worldPos[secondAxis] > -axisWidth * minimumy && worldPos[secondAxis] < axisWidth * minimumy) {
			color[firstAxis] = 1.0;
			axisDrawn = true;
		}
	}
	if (drawSecondAxis) {
		if (worldPos[firstAxis] > -axisWidth * minimumx && worldPos[firstAxis] < axisWidth * minimumx) {
			color[secondAxis] = 1.0;
			axisDrawn = true;
		}
	}
	if (axisOnly && !axisDrawn) {
		color.a = 0.0f; // Do not draw outside of axes
	}
	return color;
}

vec4 grid(vec3 worldPos, int firstAxis, int secondAxis, bool drawFirstAxis, bool drawSecondAxis)
{
	return grid(worldPos, firstAxis, secondAxis, drawFirstAxis, drawSecondAxis, false);
}

// Returns color and depths of a grid pixel
// gridType = GRID_XY, GRID_XY, GRID_YZ
// depth = depth for gl_FragCoord
// linearDepth = linear depth in range <near, far>
vec4 drawGrid(int gridType, bool drawFirstAxis, bool drawSecondAxis, bool axesOnly, out float depth, out float linearDepth)
{
	int firstAxis;
	int secondAxis;
	int otherAxis;
	if (gridType == GRID_XY) {
		firstAxis = 0;
		secondAxis = 1;
		otherAxis = 2;
	} else if (gridType == GRID_YZ) {
		firstAxis = 1;
		secondAxis = 2;
		otherAxis = 0;
	} else {
		firstAxis = 0;
		secondAxis = 2;
		otherAxis = 1;
	}

	// Figure out where camera ray intersects world plane
	float t = -nearPoint[otherAxis] / (farPoint[otherAxis] - nearPoint[otherAxis]);
	vec3 worldPos = nearPoint + t * (farPoint - nearPoint);

	// Compute depth from world position
	vec4 clipSpacePos = projectionMatrix * viewMatrix * vec4(worldPos.xyz, 1.0);
	float clipSpaceDepth = (clipSpacePos.z / clipSpacePos.w);
	depth = 0.5 + 0.5 * clipSpaceDepth;

	vec4 color = grid(worldPos, firstAxis, secondAxis, drawFirstAxis, drawSecondAxis, axesOnly) * float(t > 0);

	linearDepth = linearize_depth(clipSpaceDepth, u_near, u_far);
	float linearDepthNormalized = linearDepth / u_far;

	float fading = max(0, (0.5 - linearDepthNormalized));
	fading = smoothstep(1.0f - gridFadeOffset, 0.2f, linearDepthNormalized + gridFadeOffset);
	color.a *= fading * gridStrength;

	return color;
}

vec4 drawGrid(int gridType, bool drawFirstAxis, bool drawSecondAxis, out float depth, out float linearDepth)
{
	return drawGrid(gridType, drawFirstAxis, drawSecondAxis, false, depth, linearDepth);
}

void swap(inout float a, inout float b) {
	float tmp = a;
	a = b;
	b = tmp;
}

void swap(inout int a, inout int b) {
	int tmp = a;
	a = b;
	b = tmp;
}

void drawWboit(float linearDepth)
{
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

// Unused, perhaps for future use
void drawAllGrids() {
	int gridPlanes[3];
	gridPlanes[0] = GRID_XZ;
	gridPlanes[1] = GRID_XY;
	gridPlanes[2] = GRID_YZ;

	float depths[3];
	float linearDepths[3];
	vec4 colors[3];

	for (int i = 0; i < 3; i++) {
		float depth;
		float linearDepth;
		vec4 color;
		color = drawGrid(gridPlanes[i], true, true, depth, linearDepth);
		depths[i] = depth;
		linearDepths[i] = linearDepth;
		colors[i] = color;
	}

	float firstDepth = depths[0];
	float secondDepth = depths[1];
	float thirdDepth = depths[2];
	int firstIndex = 0;
	int secondIndex = 1;
	int thirdIndex = 2;

	// Sort the three planes by depth
	if (firstDepth > thirdDepth) {
		swap(firstDepth, thirdDepth);
		swap(firstIndex, thirdIndex);
	}
	if (firstDepth > secondDepth) {
		swap(firstDepth, secondDepth);
		swap(firstIndex, secondIndex);
	}
	if (secondDepth > thirdDepth) {
		swap(secondDepth, thirdDepth);
		swap(secondIndex, thirdIndex);
	}

	float linearDepth; // For wboit

	// Render sorted (or render only the first layer to only show in the current octant)
	if (colors[firstIndex].a > ALPHA_CUTOFF) {
		gl_FragDepth = firstDepth;
		linearDepth = linearDepths[firstIndex];
		FragColor = colors[firstIndex];
	} else if (colors[secondIndex].a > ALPHA_CUTOFF) {
		gl_FragDepth = secondDepth;
		linearDepth = linearDepths[secondIndex];
		FragColor = colors[secondIndex];
	} else {
		gl_FragDepth = thirdDepth;
		linearDepth = linearDepths[thirdIndex];
		FragColor = colors[thirdIndex];
	}

	drawWboit(linearDepth);
}

vec4 blend(vec4 a, vec4 b)
{
	return a * a.a + b * (1 - a.a);
}

void main()
{
	gridColor = u_gridColor;
	gridSize = u_gridSize;
	axisWidth = u_axisWidth;
	gridStrength = u_gridStrength;
	gridFadeOffset = u_gridFadeOffset;

	// XZ grid
	float depth;
	float linearDepth;
	vec4 color = drawGrid(GRID_XZ, false, false, depth, linearDepth);

	// Y axis
	float depthY;
	float linearDepthY;
	//vec4 colorY = drawGrid(GRID_XY, false, true, true, depthY, linearDepthY);
	vec4 colorY = drawAxis(GRID_XY, false, true, true, depthY, linearDepthY);

	gridSize *= 0.1f;
	gridStrength *= 1.5f;
	lineWidth = 2.0f;
	axisWidth = 4.6f;
	vec4 colorS = drawGrid(GRID_XZ, true, true, depth, linearDepth);

	vec4 xzGridBlended = blend(colorS, color);

	// Resolve render order
	if (false && color.a > ALPHA_CUTOFF && depth < depthY || colorY.a < ALPHA_CUTOFF) {
		gl_FragDepth = depth;
		FragColor = xzGridBlended;
	} else {
		gl_FragDepth = depthY;
		linearDepth = linearDepthY;
		FragColor = colorY;
	}
	drawWboit(linearDepth);
}
