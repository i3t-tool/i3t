#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 4) in vec3 aColor;

uniform mat4 pvmMatrix;
uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 normalMatrix;

out vec3 nearPoint;
out vec3 farPoint;

// Graciously borrowed from http://asliceofrendering.com/scene%20helper/2020/01/05/InfiniteGrid/
// Turns out not so graciously, there are some opengl x vulkan differences that caused some hiccups ;-;

vec3 UnprojectPoint(float x, float y, float z, mat4 view, mat4 projection) {
	mat4 viewInv = inverse(view);
	mat4 projInv = inverse(projection);
	vec4 unprojectedPoint = viewInv * projInv * vec4(x, y, z, 1.0);
	return unprojectedPoint.xyz / unprojectedPoint.w;
}

void main()
{
	nearPoint = UnprojectPoint(aPos.x, aPos.y, -1.0, viewMatrix, projectionMatrix).xyz;// unprojecting on the near plane
	farPoint = UnprojectPoint(aPos.x, aPos.y, 1.0, viewMatrix, projectionMatrix).xyz;// unprojecting on the far plane
	gl_Position = vec4(aPos, 1.0);
}
