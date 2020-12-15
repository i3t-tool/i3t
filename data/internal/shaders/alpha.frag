#version 140

out vec4 fragColor;

in vec3 v_normal;

uniform vec3 u_lightDirection;
uniform vec3 u_lightColor;
uniform vec3 u_diffuseColor;
uniform vec3 u_ambientColor;

uniform float u_alpha;

void main() {
	
	vec3 normal = v_normal;

	//float NdotL = max(dot(normal, -u_lightDirection), 0.0);
	//vec3 lightDiffuse = u_lightColor * NdotL;

	//vec3 finalColor = u_diffuseColor * (u_ambientColor + lightDiffuse);
	vec3 finalColor = u_diffuseColor * u_ambientColor;

	fragColor = vec4(finalColor, u_alpha);

}
