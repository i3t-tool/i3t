#version 140
//layout(location=0) 
out vec4 fragmentColor;
//out float val;

uniform sampler2D tex0;
uniform sampler2D texshadow;
uniform vec3 camera;
uniform vec4 light;

smooth in vec3 normn;//camera space normal
smooth in vec3 cpos;//camera space pos
smooth in vec3 pos;//glob pos
smooth in vec2 coord;//uv

vec3 ambient=vec3(0.25,0.25,0.35);
vec3 diffuse=vec3(0.0);
vec3 specular=vec3(0.0);
vec3 lvec;
vec3 cpos2=pos+camera;

void main(){
	fragmentColor=texture(tex0,vec2(coord.x,-coord.y),-0.5);
	ambient=ambient*fragmentColor.rgb;

	lvec=vec3(0.5,1.0,-1.0);				//dir
	lvec=normalize(lvec);
	diffuse=fragmentColor.rgb*max(dot(lvec,normn),0.0);		
	specular=vec3(0.5,0.5,0.3)*pow(max(dot( reflect(-lvec,normn) , normalize(-cpos2) ),0.0),4.0);


	fragmentColor=vec4((ambient+diffuse+specular),fragmentColor.a);

	float fog=pow(0.95,max(pos.y,0.0)*0.7);			//fog
	fog=fog*( 1.0-pow(0.95,dot(cpos,cpos)*dot(cpos,cpos)/300000.0) );
	fragmentColor=(1.0-fog)*fragmentColor+fog*vec4(0.8,0.8,0.8,fragmentColor.a);
	
	//val=gl_FragCoord.z;
	//val=fragmentColor.r;
	
	
	if( fragmentColor.a<0.5){discard;}	
}
