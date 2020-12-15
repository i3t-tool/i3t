#version 140

out vec4 fragColor;

in vec2 v_texCoord0;
in vec3 v_normal;

uniform sampler2D u_diffuse;
uniform vec3 u_lightDirection;
uniform vec3 u_lightColor;
uniform vec3 u_diffuseColor;
uniform vec3 u_ambientColor;

void main() {
	
	vec3 normal = normalize(v_normal);

	float NdotL = max(dot(normal, -u_lightDirection), 0.0);
	vec3 lightDiffuse = u_lightColor * NdotL;

	vec3 diffuse = texture(u_diffuse, v_texCoord0).rgb * u_diffuseColor;

	vec3 finalColor = diffuse * (u_ambientColor + lightDiffuse);

	fragColor = vec4(finalColor, 1.0);

}
