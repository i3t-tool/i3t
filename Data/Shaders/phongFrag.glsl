#version 330 core

//out vec4 FragColor;

layout (location = 0) out vec4 FragColor;

// WBOIT
uniform bool u_wboitFlag = false;
uniform int u_wboitFunc = 0;
uniform float u_wboitNear = 0.1;
uniform float u_wboitFar = 500;
layout (location = 1) out vec4 AccumulationBuffer;
layout (location = 2) out float RevealageBuffer;
// END WBOIT

in vec3 FragPos;
in vec2 TexCoords;
in vec3 Normal;
in vec3 Tangent;
in vec3 Binormal;

uniform float u_opacity = 1.0;
uniform vec3 u_tint = vec3(1.0);
uniform int u_lightingModel = 0;

uniform float alphaCutoff = 0.1;

uniform mat4 viewMatrix;

// Textures ====================================

uniform float normalStrength;

uniform sampler2D    diffuse0;
uniform bool        diffuse0_active = false;

uniform sampler2D    specular0;
uniform bool        specular0_active = false;

uniform sampler2D    normal0;
uniform bool        normal0_active = false;

uniform sampler2D    ao0;
uniform bool        ao0_active = false;

uniform sampler2D    emission0;
uniform bool        emission0_active = false;

// Material ====================================

struct Material {
	vec3 diffuse;
	vec3 specular;
	vec3 ambient;
	float shininess;
};

uniform Material material;

// Lights ======================================

struct PointLight {
	vec3 position;

	float radius;

	float intensity;
	vec3 color;
	vec3 specular;
};

struct SunLight {
	vec3 direction;

	float intensity;
	vec3 color;
	vec3 specular;
};

struct SpotLight {
	vec3 position;
	vec3 direction;

	float cutoffAngle;
	float cutoffSoftAngle;
	float radius;

	float intensity;
	vec3 color;
	vec3 specular;
};

#define MAX_POINT_LIGHTS 20
#define MAX_SUN_LIGHTS 8
#define MAX_SPOT_LIGHTS 10

uniform int pointLightsCount;
uniform PointLight pointLights[MAX_POINT_LIGHTS];

uniform int sunLightsCount;
uniform SunLight sunLights[MAX_SUN_LIGHTS];

uniform int spotLightsCount;
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];

float map(float value, float min1, float max1, float min2, float max2) {
	return min2 + (value - min1) * (max2 - min2) / (max1 - min1);
}

float easeOutPower(float x, float power) {
	return 1.0 - pow(1.0 - x, power);
}

vec2 texCoords() {
	vec2 coords = TexCoords;
	//Function for possible further modification of tex coords
	return coords;
}

float random(vec2 st) { return fract(sin(dot(st.xy, vec2(12.9898, 78.233))) * 43758.5453123); }

float linearize_depth(float d, float zNear, float zFar)
{
	float z_n = 2.0 * d - 1.0;
	return 2.0 * zNear * zFar / (zFar + zNear - z_n * (zFar - zNear));
}

vec3 calculateAmbientLight(vec3 lightColor, vec3 ambient) {
	return lightColor * ambient * (diffuse0_active ? vec3(texture(diffuse0, texCoords())) : vec3(1)) * u_tint;
}

vec3 calculateDiffuseLight(vec3 lightColor, vec3 diffuse, vec3 N, vec3 L) {
	vec3 res = lightColor * diffuse * max(0.0, dot(N, L));
	res *= (diffuse0_active ? vec3(texture(diffuse0, texCoords())) : vec3(1));
	res *= (ao0_active ? vec3(texture(ao0, texCoords()).r) : vec3(1));//Using just the RED channel for AO (for gltf)
	res += (emission0_active ? vec3(texture(emission0, texCoords())) : vec3(0));
	res *= u_tint;
	return res;
}

vec3 calculateNormalMapping(vec3 normal, vec3 tangent, vec3 binormal) {
	if (normal0_active && normalStrength > 0) {
		vec3 mapNormal = texture(normal0, texCoords()).rgb;
		//Apply normal strength by just mixing with a neutral normal
		//This can only make normals "weaker"
		mapNormal = mix(vec3(0.5, 0.5, 1), mapNormal, normalStrength);
		mapNormal = normalize(mapNormal * 2.0 - 1.0);// from [0, 1] to [-1, 1]
		normal = normalize(mat3(tangent, binormal, normal) * mapNormal);// from tangent space to view space
	}
	return normal;
}

float calculateAttenuation(float dist, float radius) {
	float constant = 1.0;
	float linear = 0.09f;
	float quadratic = 0.032f;

	//float attenuation = clamp(1.0 / (1.0 + 0.0 * lightDist + 1.0 * lightDist * lightDist), 0.0, 1.0);
	//float attenuation = pow(clamp(1.0 - dist*dist/(radius*radius), 0.0, 1.0), 2);
	float attenuation = pow(clamp(1.0 - dist/(radius), 0.0, 1.0), 2);
	//float attenuation = 1.0 / (constant + linear * dist + quadratic * (dist * dist));
	return attenuation;
}

vec3 phongSpecular(vec3 lightColor, vec3 specular, vec3 L, vec3 V, vec3 N, vec3 R, float shininess) {
	float specDot = max(dot(R, V), 0.0);
	// Using the GREEN channel for specular texture (for gltf)
	float specularTextureFactor = (specular0_active ? texture(specular0, texCoords()).g : 1.0);
	return lightColor * specular * pow(specDot, shininess) * specularTextureFactor * u_tint;
}

vec3 blinnPhongSpecular(vec3 lightColor, vec3 specular, vec3 L, vec3 V, vec3 N, float shininess) {
	vec3 H = normalize(L + V);// Halfway vector
	float specDot = max(dot(N, H), 0.0);
	// Assuming a closed object thus normal facing away from the light should NOT have any specular presents, helps with artifacts
	float facingAwayFactor = max(dot(N, L), 0.0);
	float facingAwayFactorAdj = easeOutPower(facingAwayFactor, 6.0);
	// Using the GREEN channel for specular texture (for gltf)
	float specularTextureFactor = (specular0_active ? texture(specular0, texCoords()).g : 1.0);
	return lightColor * specular * pow(specDot, shininess) * facingAwayFactorAdj * specularTextureFactor * u_tint;
}

vec3 calculateSpecularLight(vec3 lightColor, vec3 specular, vec3 L, vec3 V, vec3 N, vec3 R, float shininess) {
	if (u_lightingModel == 1) {
		if (!gl_FrontFacing) { // No specular on backfaces (suppresses artefacts)
			return vec3(0);
		}
		return blinnPhongSpecular(lightColor, specular, L, V, N, shininess);
	} else {
		return phongSpecular(lightColor, specular, L, V, N, R, shininess);
	}
}

vec3 calculateSunLight(SunLight light, Material material, vec3 fragPos, vec3 normal, vec3 tangent, vec3 binormal) {
	if (light.intensity <= 0) {
		return vec3(0);
	}

	normal = calculateNormalMapping(normal, tangent, binormal);

	vec3 lightDir = (viewMatrix * vec4(light.direction, 0.0)).xyz;
	vec3 fragToLight = -lightDir;
	vec3 L = normalize(fragToLight);
	vec3 N = normalize(normal);
	vec3 R = reflect(-L, N);
	vec3 V = normalize(-fragPos);

	vec3 ambientLight = calculateAmbientLight(light.color, material.ambient);
	vec3 diffuseLight = calculateDiffuseLight(light.color, material.diffuse, N, L);
	vec3 specularLight = calculateSpecularLight(light.color, material.specular, L, V, N, R, material.shininess);
	specularLight *= light.specular;

	//specularLight *= 0.3f;//Turn down sun specular a bit

	return light.intensity * (ambientLight + diffuseLight + specularLight);
}

vec3 calculateSpotLight(SpotLight light, Material material, vec3 fragPos, vec3 normal, vec3 tangent, vec3 binormal) {
	if (light.intensity <= 0) {
		return vec3(0);
	}

	normal = calculateNormalMapping(normal, tangent, binormal);

	vec3 lightPos = (viewMatrix * vec4(light.position, 1.0)).xyz;
	vec3 lightDir = (viewMatrix * vec4(light.direction, 0.0)).xyz;

	vec3 fragToLight = lightPos - fragPos;
	float lightDist = length(fragToLight);
	float lightRadius = light.radius;
	vec3 L = normalize(fragToLight);
	vec3 N = normalize(normal);
	vec3 R = reflect(-L, N);
	vec3 V = normalize(-fragPos);
	vec3 S = normalize(lightDir);

	vec3 ambientLight = calculateAmbientLight(light.color, material.ambient);
	vec3 diffuseLight = calculateDiffuseLight(light.color, material.diffuse, N, L);
	vec3 specularLight = calculateSpecularLight(light.color, material.specular, L, V, N, R, material.shininess);
	specularLight *= light.specular;

	vec3 outColor = vec3(0);

	float fragAngle = radians(90.0f) * max(0.0, dot(-L, S));
	if (fragAngle >= light.cutoffAngle) {
		float attenuation = calculateAttenuation(lightDist, lightRadius);
		float softFlatDiff = light.cutoffSoftAngle - light.cutoffAngle;
		float softFactor = clamp((fragAngle - light.cutoffAngle) / softFlatDiff, 0.0, 1.0);
		outColor = smoothstep(0.0, 1.0, softFactor) * light.intensity * attenuation * (ambientLight + diffuseLight + specularLight);
	} else {
		outColor *= 0;
	}
	return outColor;
}

vec3 calculatePointLight(PointLight light, Material material, vec3 fragPos, vec3 normal, vec3 tangent, vec3 binormal) {
	if (light.intensity <= 0) {
		return vec3(0);
	}

	normal = calculateNormalMapping(normal, tangent, binormal);

	vec3 lightPos = (viewMatrix * vec4(light.position, 1.0)).xyz;
	vec3 fragToLight = lightPos - fragPos;
	float lightDist = length(fragToLight);
	float lightRadius = light.radius;
	vec3 L = normalize(fragToLight);
	vec3 N = normalize(normal);
	vec3 R = reflect(-L, N);
	vec3 V = normalize(-fragPos);

	vec3 ambientLight = calculateAmbientLight(light.color, material.ambient);
	vec3 diffuseLight = calculateDiffuseLight(light.color, material.diffuse, N, L);
	vec3 specularLight = calculateSpecularLight(light.color, material.specular, L, V, N, R, material.shininess);
	specularLight *= light.specular;

	float attenuation = calculateAttenuation(lightDist, lightRadius);
	return light.intensity * attenuation * (ambientLight + diffuseLight + specularLight);
}

void main() {
	float alpha = 1.0;
	if (diffuse0_active) {
		alpha = texture(diffuse0, texCoords()).a;
	}
	alpha = min(alpha, u_opacity);

	vec3 outColor = vec3(0);
	for (int i = 0; i < sunLightsCount; i++) {
		outColor += calculateSunLight(sunLights[i], material, FragPos, Normal, Tangent, Binormal);
	}
	for (int i = 0; i < pointLightsCount; i++) {
		outColor += calculatePointLight(pointLights[i], material, FragPos, Normal, Tangent, Binormal);
	}
	for (int i = 0; i < spotLightsCount; i++) {
		outColor += calculateSpotLight(spotLights[i], material, FragPos, Normal, Tangent, Binormal);
	}

	float gamma = 1.1;
	float exposure = 1.6;

	vec3 mapped = vec3(1.0) - exp(-outColor * exposure);// HDR correction
	mapped = pow(mapped, vec3(1.0 / gamma));// gamma correction

	FragColor = vec4(mapped, alpha);

	//float linearDepth = linearize_depth(gl_FragCoord.z, u_wboitNear, u_wboitFar);
	//float wboitDepth = map(linearDepth, u_wboitNear, u_wboitFar, 0.1, 500.0);
	//FragColor = vec4(vec3(gl_FragCoord.z, linearDepth, gl_FragCoord.z / gl_FragCoord.w), 1.0);

	// WBOIT
	if (u_wboitFlag) {
		//////// Customize if needed /////////
		float fragCoordZ = gl_FragCoord.z;
		float wboitDepth = 1.0;
		if (u_wboitFunc != 0) {
			wboitDepth = 2.0 * u_wboitNear * u_wboitFar / (u_wboitFar + u_wboitNear - (2.0 * gl_FragCoord.z - 1.0) * (u_wboitFar - u_wboitNear));
		}
		//////////////////////////////////////
		// Branching based on which weight function is selected
		float weight = 1.0;
		switch (u_wboitFunc) {
			case 0:// Off
			weight = 1.0;
			break;
			case 1:// Wboit paper Eq. 7
			weight = FragColor.a * max(1e-2, min(3e3, 10/(1e-5 + pow(wboitDepth/5, 2) + pow(wboitDepth/200, 6))));
			break;
			case 2:// Wboit paper Eq. 8
			weight = FragColor.a * max(1e-2, min(3e3, 10/(1e-5 + pow(wboitDepth/10, 3) + pow(wboitDepth/200, 6))));
			break;
			case 3:// Wboit paper Eq. 9
			weight = FragColor.a * max(1e-2, min(3e3, 0.03/(1e-5 + pow(wboitDepth/200, 4))));
			break;
			case 4:// Wboit paper Eq. 10
			weight = FragColor.a * max(1e-2, 3e3 * pow(1 - fragCoordZ, 3));
			break;
			case 5:// LearnOpengl example 1 (Eq. 9 with color bias)
			weight = max(min(1.0, max(max(FragColor.r, FragColor.g), FragColor.b) * FragColor.a), FragColor.a) * clamp(0.03 / (1e-5 + pow(wboitDepth / 200, 4.0)), 1e-2, 3e3);
			break;
			case 6:// LearnOpengl example 2 (Eq. 10 with changes)
			weight = clamp(pow(min(1.0, FragColor.a * 10.0) + 0.01, 3.0) * 1e8 * pow(1.0 - fragCoordZ * 0.9, 3.0), 1e-2, 3e3);
			break;
			case 7:// z^-3
			weight = pow(wboitDepth, -3);
			break;
			case 8:// abs(z - zFar + Eps)
			weight = abs(wboitDepth - u_wboitFar + 0.00001);
			break;
		}
		// Store accumulation color (RGB) and revealage factor (A)
		AccumulationBuffer = vec4(FragColor.rgb * FragColor.a * weight, FragColor.a);
		// Store accumulation alpha (R)
		RevealageBuffer = FragColor.a * weight;
	}
	// END WBOIT
}