#version 140
//#extension GL_ARB_conservative_depth : enable
out vec4 fragmentColor;
//layout (depth_less) out float gl_FragDepth;

uniform sampler2D tex0;
uniform vec3 camera;
uniform vec4 light;
uniform vec4 color=vec4(1.0,1.0,1.0,1.0);
uniform vec3 ldir=vec3(0.25,1.0,0.5);

smooth in vec3 normn;//camera space normal
smooth in vec2 coord;//uv

void main(){
	//fragmentColor=texture(tex0,vec2(coord.x,coord.y),-0.5)*color;
	fragmentColor=color;
	
	vec3 lvec=normalize(ldir);
	fragmentColor.rgb=mix(fragmentColor.rgb,fragmentColor.rgb*max(dot(lvec,normn),0.0),0.2);
	
}
