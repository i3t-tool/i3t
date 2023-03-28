#version 330 core

layout (location = 0) out vec4 FragColor;

uniform sampler2D accum;
uniform sampler2D reveal;

const float EPSILON = 0.00001f;

// calculate floating point numbers equality accurately
bool isApproximatelyEqual(float a, float b)
{
	return abs(a - b) <= (abs(a) < abs(b) ? abs(b) : abs(a)) * EPSILON;
}

// get the max value between three values
float max3(vec3 v)
{
	return max(max(v.x, v.y), v.z);
}

void main()
{
	// fragment coordinates
	ivec2 coords = ivec2(gl_FragCoord.xy);

	// fragment revealage
	//float revealage = texelFetch(reveal, coords, 0).r;

	// Unpack buffers into accumulation and revealage
	vec4 accumulation = texelFetch(accum, coords, 0);
	float revealage = accumulation.a;
	accumulation.a = texelFetch(reveal, coords, 0).r;

	// save the blending and color texture fetch cost if there is not a transparent fragment
	if (isApproximatelyEqual(revealage, 1.0f)) {
		discard;
	}

	// suppress overflow
	if (isinf(max3(abs(accumulation.rgb)))) {
		accumulation.rgb = vec3(accumulation.a);
	}

	// prevent floating point precision bug
	vec3 average_color = accumulation.rgb / max(accumulation.a, EPSILON);

	// blend pixels
	// FragColor = vec4(average_color, clamp((1.0f - revealage) * 0.5f, 0.0f, 1.0f));
	//float val = 1.0f - revealage;
	//FragColor = vec4(vec3(val), 0.5);
	// FragColor = vec4(average_color, 0.5);

	FragColor = vec4(average_color, 1.0f - revealage);
	//FragColor = vec4(accumulation.rgb, 1.0);

//	if (revealage > 1.0f) {
//		FragColor = vec4(1.0, 0.0, 0.0, 1.0);
//	} else {
//		FragColor = vec4(0.0, 1.0, 0.0, 1.0);
//	}
}