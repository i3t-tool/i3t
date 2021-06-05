#version 140
#extension GL_ARB_conservative_depth : enable
in vec3 position;
in vec3 norm;

uniform mat4  PVMmatrix;   // transformation matrix
uniform mat4 VNmatrix;
uniform mat4 Mmatrix;

uniform mat4 P2Matrix;

smooth out vec3 normn;//camera space normal

void main(){
	mat4 P2adj=P2Matrix;
	P2adj[0][0]=1/P2adj[0][0];
	P2adj[1][1]=1/P2adj[1][1];
	P2adj[2][2]=1/P2adj[2][2];

	gl_Position=(P2Matrix)*vec4(position.xyz,1.0);
	gl_Position/=gl_Position[3];

	gl_Position=PVMmatrix*gl_Position;


	normn=(VNmatrix*vec4(norm,0.0)).xyz;//norm
	normn=normalize(normn);

}
