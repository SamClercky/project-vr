#version 330 core

out vec4 FragColor;

//struct Material {
//	sampler2D diffuseMap;
//	sampler2D specularMap;
//	float shininess;
//};

struct Light {
	vec3 position;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;

    mat4 lightTransform;
    sampler2D shadowMap;
};

in VS_OUT {
    vec3 position;
    vec3 normal;
    vec2 texCoord;
    vec4 lightPositions[16];
} fs_in;

uniform vec3 lightColor = vec3(1.0f);
uniform sampler2D ourTexture;

uniform vec3 viewPos;
//uniform Material material;
uniform Light lights[];
uniform int numLights;

float shadowCalculation(vec4 lPosition, in sampler2D shadowMap);
vec3 CalcDirLight(int lightId, vec3 normal, vec3 viewDir, vec4 lPosition);

float shadowCalculation(vec4 lPosition, in sampler2D shadowMap) {
    vec3 shadowCoords = lPosition.xyz / lPosition.w; // maintain compotibility with other perspectives
    shadowCoords = shadowCoords * .5f + .5f; // between [0,1]
    float depth = shadowCoords.z;
    float shadowBias = .005f;
    vec2 texelSize = 1.f / textureSize(shadowMap, 0);
    float shadow = 0;
    for (int x = -1; x <= 1; x++) {
        for (int y = -1; y <= 1; y++) {
            float pcfDepth = texture(shadowMap, shadowCoords.xy + vec2(x, y) * texelSize).r; // blur kernel
            shadow += depth - shadowBias < pcfDepth ? 1.f : 0.f;
        }
    }
    shadow /= 9.f;

    if (depth > 1.f) {
        shadow = 1.f;
    }

    return shadow;
}

void main() {
	vec3 norm = normalize(fs_in.normal);
	vec3 viewDir = normalize(viewPos - fs_in.position);

	//phase 1: directional lighting
    vec3 result = vec3(0.0f);
    for (int i=0; i<numLights && i < 16; i++) {
        result += CalcDirLight(i, norm, viewDir, fs_in.lightPositions[i]);
    }

	FragColor = texture(ourTexture, fs_in.texCoord) * vec4(result, 1.0);
}

vec3 CalcDirLight(int lightId, vec3 normal, vec3 viewDir, vec4 lPosition) {
    vec3 lightDir = normalize(-lights[lightId].direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 0.5f);//shine
    // combine results
    vec3 ambient = lights[lightId].ambient; //* vec3(texture(material.diffuseMap, vTexCoord));
    vec3 diffuse = lights[lightId].diffuse * diff; //* vec3(texture(material.diffuseMap, vTexCoord));
    vec3 specular = lights[lightId].specular * spec; //* vec3(texture(material.specularMap, vTexCoord));
    float isShadow = shadowCalculation(lPosition, lights[lightId].shadowMap);
    return (ambient + diffuse*isShadow + specular*isShadow);
}