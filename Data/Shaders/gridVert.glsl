#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 4) in vec3 aColor;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

uniform mat4 modelMatrix;
uniform mat4 u_modelMatrixInv;

out vec4 nearPointG;
out vec4 farPointG;

// Graciously borrowed from http://asliceofrendering.com/scene%20helper/2020/01/05/InfiniteGrid/
// Turns out not so graciously, there are some opengl x vulkan differences that caused some hiccups ;-;

vec4 UnprojectPoint(float x, float y, float z, mat4 view, mat4 projection) {
	mat4 viewInv = inverse(view);
	mat4 projInv = inverse(projection);
	vec4 unprojectedPoint = viewInv * projInv * vec4(x, y, z, 1.0);
	return unprojectedPoint / unprojectedPoint.w;
}

void main()
{
	nearPointG = UnprojectPoint(aPos.x, aPos.y, -1.0, viewMatrix, projectionMatrix);// unprojecting on the near plane
	farPointG = UnprojectPoint(aPos.x, aPos.y, 1.0, viewMatrix, projectionMatrix);// unprojecting on the far plane
#ifdef GENERIC_GRID
	nearPointG = modelMatrix * nearPointG;
	farPointG = modelMatrix * farPointG;
#endif
	gl_Position = vec4(aPos, 1.0);
}
