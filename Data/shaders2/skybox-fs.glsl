#version 140
out vec4 fragmentColor;

uniform sampler2D tex0;
smooth in vec2 coord;//uv

void main(){
	fragmentColor=vec4(texture(tex0,vec2(coord.x,-coord.y)).xyz,1.0);
	gl_FragDepth=0.999999;	
}
