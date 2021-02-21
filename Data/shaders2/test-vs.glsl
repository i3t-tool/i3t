#version 140
in vec3 position;
in vec2 uv;

uniform mat4  VMmatrix;   // transformation matrix
uniform mat4 VNmatrix;
uniform mat4 Pmatrix;

void main(){
  gl_Position = vec4(position.xy,1.0, 1.0);
}
