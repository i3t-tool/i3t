#version 140
in vec3 position;
in vec3 norm;
in vec2 uv;

uniform mat4  PVMmatrix;   // transformation matrix
uniform mat4 VNmatrix;
uniform mat4 Mmatrix;

smooth out vec3 normn;//camera space normal
smooth out vec2 coord;//uv

void main(){
	coord=uv;//coord
	gl_Position=PVMmatrix*vec4(position, 1.0);

	normn=(VNmatrix*vec4(norm,0.0)).xyz;//norm
	normn=normalize(normn);
}
