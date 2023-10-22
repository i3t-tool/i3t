#version 140
out vec4 fragmentColor;

uniform sampler2D tex0;
uniform vec3 camera;
uniform vec4 light;
uniform vec4 color=vec4(1.0,1.0,1.0,1.0);
uniform vec3 ldir=vec3(0.25,1.0,0.5);

//smooth in vec3 normn;//camera space normal
smooth in vec3 n;//camera space normal
smooth in vec3 pos;//glob pos
smooth in vec2 coord;//uv

vec3 ambient=vec3(0.35,0.35,0.5);
vec3 diffuse=vec3(0.0);
vec3 specular=vec3(0.0);				//dir
vec3 cpos=pos+camera;

void main(){
	vec3 lvec=normalize(ldir);
	vec3 normn = normalize(n); //PF
	fragmentColor=texture(tex0,vec2(coord.x,coord.y),-0.5)*color;

	ambient= ambient*fragmentColor.rgb;
	diffuse= fragmentColor.rgb*max(dot(lvec,normn),0.0);
	specular=vec3(0.3,0.3,0.1)*pow(max(dot( reflect(-lvec,normn) , normalize(-cpos) ),0.0),4.0);

	fragmentColor=vec4((ambient+diffuse+specular),fragmentColor.a);


	if( fragmentColor.a<0.5){discard;}
}