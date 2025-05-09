#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 4) in vec3 aColor;

uniform mat4 pvmMatrix;
uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 normalMatrix;

uniform bool useSingleColor = false;
uniform vec3 singleColor;

// Projection (View) inverse
uniform mat4 projectionInverseMatrix;

out vec3 Color;
out vec3 ClipPos;

void main()
{
	ClipPos = aPos;

	gl_Position = projectionInverseMatrix * vec4(aPos, 1.0);
	gl_Position /= gl_Position.w;

	gl_Position = pvmMatrix * gl_Position;
	Color = useSingleColor ? singleColor : aColor;
}