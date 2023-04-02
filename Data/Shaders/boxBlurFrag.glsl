#version 330 core

layout (location = 0) out vec4 FragColor;

uniform sampler2D source;

uniform int u_kernelSize = 10;
uniform bool u_vertical = true;

uniform vec2 u_resolution;

// Two pass box blur shader, expanded to handle alpha
// Orginal credit to: https://www.shadertoy.com/view/llGSz3

float divideByAlpha(float color, float alpha)
{
	if (color <= 0.001f) {
		return 0.0;
	}
	if (alpha <= 0.001f) {
		return 0.0;
	}
	return color / alpha;
}

vec4 divideByAlphaV(vec4 v)
{
	return vec4(divideByAlpha(v.r, v.a), divideByAlpha(v.g, v.a), divideByAlpha(v.b, v.a), v.a);
}

void main()
{
	vec2 uv = gl_FragCoord.xy / u_resolution.xy;
	float pixelSize = 1.0 / (u_vertical ? u_resolution.y : u_resolution.x);

	// Box blur
	vec4 accumulation = vec4(0);
	vec4 weightSum = vec4(0);
	for (float i = -u_kernelSize; i <= u_kernelSize; i++) {
		vec2 offset = uv + (u_vertical ? vec2(0.0, i * pixelSize) : vec2(i * pixelSize, 0.0));
		vec4 sourceRGBA = texture(source, offset);
		// Premultiply alpha to blur transparent pixels properly
		sourceRGBA = vec4(sourceRGBA.rgb * sourceRGBA.a, sourceRGBA.a);
		accumulation += sourceRGBA;
		weightSum += 1.0;
	}
	vec4 sum = accumulation / weightSum;

	// Unpremuliply alpha
	// The blur pass creates rgb values that are already premultiplied by alpha
	// (or rather the colors are dimmed the same way alpha is)
	// In order to preserve colors for later compositing/blending, original color is restored by dividing
	// the rgb elements with the their alpha. Just a simple divide with some edge cases resolved.
	FragColor = divideByAlphaV(sum);
}
