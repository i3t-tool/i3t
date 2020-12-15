#version 140

in vec3 a_position;
in vec3 a_normal;

uniform mat4 u_PVM;
uniform mat3 u_normalTrans;

out vec3 v_normal;

void main() {

	v_normal = u_normalTrans * a_normal;	
	gl_Position = u_PVM * vec4(a_position, 1.0);
}
