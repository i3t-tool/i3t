#version 140
in vec3 position;
in vec3 norm;
in vec2 uv;

uniform mat4  VMmatrix;   // transformation matrix
uniform mat4 VNmatrix;
uniform mat4 Pmatrix;
uniform mat4 Mmatrix;

smooth out vec3 normn;//camera space normal
smooth out vec3 cpos;//camera space pos
smooth out vec3 pos;//model space pos
smooth out vec2 coord;//uv

void main(){
	pos=(Mmatrix*vec4(position,1.0)).xyz;
	coord=uv;//coord
	gl_Position=VMmatrix*vec4(position, 1.0);
	cpos=gl_Position.xyz;//cpos
	gl_Position=Pmatrix*gl_Position;
	normn=(VNmatrix*vec4(norm,0.0)).xyz;//norm
	normn=normalize(normn);
}
