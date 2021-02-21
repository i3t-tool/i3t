#version 140
in vec3 position;
in vec3 norm;

uniform mat4  VMmatrix;   // transformation matrix
uniform mat4 VNmatrix;
uniform mat4 Pmatrix;
uniform mat4 Mmatrix;

smooth out vec3 normn;//camera space normal

void main(){
	gl_Position=Pmatrix*VMmatrix*vec4(position, 1.0);

	normn=(VNmatrix*vec4(norm,0.0)).xyz;//norm
	normn=normalize(normn);
}
