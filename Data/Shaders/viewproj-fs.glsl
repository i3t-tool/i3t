#version 140
//#extension GL_ARB_conservative_depth : enable

out vec4 fragmentColor;
//layout (depth_less) out float gl_FragDepth;

uniform sampler2D tex0;
uniform vec3 camera;
uniform vec4 light;
uniform vec3 ldir=vec3(0.25,1.0,0.5);

uniform vec4 color=vec4(1.0,1.0,1.0,1.0);

//smooth in vec3 normn;//camera space normal
smooth in vec3 n;//camera space normal

void main(){
	fragmentColor=color;

	vec3 lvec=normalize(ldir);
	vec3 normn = normalize(n); //PF
	fragmentColor.rgb=fragmentColor.rgb*max(dot(lvec,normn),0.0);  //todo - clean-up this
	fragmentColor.rgb=mix(fragmentColor.rgb,color.rgb,0.7);
}