#version 140
#extension GL_ARB_conservative_depth : enable
in vec3 position;
in vec3 norm;

uniform mat4  VMmatrix;   // transformation matrix
uniform mat4 VNmatrix;
uniform mat4 Pmatrix;
uniform mat4 Mmatrix;

uniform mat4 P2Matrix;
/*uniform mat4 P2Matrix=mat4(	vec4(1.7,0	,0	,0),
							vec4(0	,1.7,0	,0),
							vec4(0	,0	,-1.2,-1.1),
							vec4(0	,0	,-1.0,0.0));*/

smooth out vec3 normn;//camera space normal

void main(){
	mat4 P2adj=P2Matrix;
	P2adj[0][0]=1/P2adj[0][0];
	P2adj[1][1]=1/P2adj[1][1];
	P2adj[2][2]=1/P2adj[2][2];
	//gl_Position=P2adj*vec4(position.xy,(position.z+1.0)*1.0,1.0);
	gl_Position=(P2Matrix)*vec4(position.xyz,1.0);
	gl_Position/=gl_Position[3];
	//gl_Position=vec4(-gl_Position.z,gl_Position.y,gl_Position.x,1.0);
	gl_Position=VMmatrix*gl_Position;
	
	//gl_Position=Pmatrix*gl_Position;
	gl_Position=Pmatrix*gl_Position;


	normn=(VNmatrix*vec4(norm,0.0)).xyz;//norm
	normn=normalize(normn);

}
