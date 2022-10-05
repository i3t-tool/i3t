#version 450

in vec3 position;
in vec3 normal;
in vec2 texCoord;

layout(location = 3) out vec3 v_Position;
layout(location = 4) out vec2 v_UV;
layout(location = 5) out vec3 v_Normal;

uniform mat4 PVMmatrix;
uniform mat4 Vmatrix;
uniform mat4 Mmatrix;

void main()
{
    v_Normal = mat3(transpose(inverse(Mmatrix))) * normal;
    vec4 worldPosition = Mmatrix * vec4(position, 1.0);
    v_Position = vec3(worldPosition);
    v_UV = texCoord;
    gl_Position = PVMmatrix * vec4(position, 1.0);
}
