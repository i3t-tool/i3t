#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;

uniform mat4 pvmMatrix;
uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 normalMatrix;

out vec3 FragPos;
out vec2 TexCoords;
out vec3 Normal;
out vec3 Tangent;
out vec3 Binormal;

//Normal mapping stuff largely inspired from 
//https://lettier.github.io/3d-game-shaders-for-beginners/normal-mapping.html
//and
//https://learnopengl.com/Advanced-Lighting/Normal-Mapping

void main() {
	gl_Position = pvmMatrix * vec4(aPos, 1.0);

	FragPos = (viewMatrix * modelMatrix * vec4(aPos, 1.0)).xyz;
	
	Normal = normalize(viewMatrix * normalMatrix * vec4(aNormal, 0.0)).xyz;
	
	//Remaining view space normal basis vectors, used to convert from tangent space to view space in frag shader
	Tangent = normalize(viewMatrix * normalMatrix * vec4(aTangent, 0.0)).xyz;
	Tangent = normalize(Tangent - dot(Tangent, Normal) * Normal); //Gram–Schmidt process
	Binormal = cross(Normal, Tangent); //Get third basis vector as a cross of the two existing orthogonal ones

	TexCoords = aTexCoords;
}