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

out vec3 Color;

void main()
{
    gl_Position = pvmMatrix * vec4(aPos, 1.0);
	Color = useSingleColor ? singleColor : aColor;
}    