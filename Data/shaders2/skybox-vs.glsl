#version 140
in vec3 position;
in vec2 uv;

uniform mat4  VMmatrix;   // transformation matrix
uniform mat4 VNmatrix;
uniform mat4 Pmatrix;

smooth out vec2 coord;//uv

void main(){
  coord=vec2(uv.xy);//coord
  gl_Position = VMmatrix * vec4(position, 1.0);
  gl_Position -=vec4(VMmatrix[3][0],VMmatrix[3][1],VMmatrix[3][2],0.0f);
  gl_Position=Pmatrix*gl_Position;
}
