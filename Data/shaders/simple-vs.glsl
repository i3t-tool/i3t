#version 140
in vec3 position;
in vec3 norm;
in vec2 uv;

uniform mat4  PVMmatrix;   // transformation matrix
uniform mat4 VNmatrix;
uniform mat4 Mmatrix;

//smooth out vec3 normn;//camera space normal
smooth out vec3 n;//camera space normal
smooth out vec3 pos;//model space pos
smooth out vec2 coord;//uv

void main(){
	pos=(Mmatrix*vec4(position,1.0)).xyz;
	coord=uv;//coord
	gl_Position=PVMmatrix*vec4(position, 1.0);
	n=(VNmatrix*vec4(norm,0.0)).xyz;
	//normn=(VNmatrix*vec4(norm,0.0)).xyz;//norm
	//normn=normalize(normn);  //PF - normal length is shortened during interpolation - must be normaized in FS
}
