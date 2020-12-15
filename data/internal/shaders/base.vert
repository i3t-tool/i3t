#version 140

in vec3 a_position;
in vec3 a_normal;
in vec2 a_texCoord0;

uniform mat4 u_PVM;
uniform mat3 u_normalTrans;

out vec2 v_texCoord0;
out vec3 v_normal;

void main() {

	v_normal = u_normalTrans * a_normal;

	v_texCoord0 = a_texCoord0;
	
	gl_Position = u_PVM * vec4(a_position, 1.0);
}