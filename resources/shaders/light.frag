#version 330 core

out vec4 FragColor;

struct Material {
	sampler2D diffuseMap;
	sampler2D specularMap;
	float shininess;
};

struct Light {
	vec3 position;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;

    float cutOff;
    float outerCutOff;

    mat4 lightTransform;
    sampler2D shadowMap;
};

in VS_OUT {
    vec3 position;
    vec3 normal;
    vec2 texCoord;
    vec3 viewPosition;
    vec4 lightPositions[16];
} fs_in;

uniform vec3 lightColor = vec3(1.0f);
uniform sampler2D ourTexture;

//uniform Material material;
uniform Light lights[];
uniform int numLights;

float shadowCalculation(vec4 lPosition, in sampler2D shadowMap);
vec3 CalcDirLight(int lightId, vec3 normal, vec3 viewDir, vec4 lPosition);
vec3 CalcPointLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir);

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
    vec3 viewDir = normalize(fs_in.viewPosition - fs_in.position);

    vec3 result = vec3(0.0f);
    for (int i = 0; i < numLights && i < 16; i++) {
        if (l.cutOff == 0.0) {
            result += CalcPointLight(l, norm, vPosition, viewDir);
        } else {
            result += CalcSpotLight(l, norm, vPosition, viewDir);
        }
        result += CalcDirLight(i, norm, viewDir, fs_in.lightPositions[i]);
    }

    FragColor = texture(ourTexture, fs_in.texCoord) * vec4(result, 1.0);
}

vec3 CalcPointLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);
    //diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    //specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 0.5); //shininess
    //attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear*distance + light.quadratic*(distance*distance));
    // combine
    vec3 ambient = light.ambient * vec3(texture(ourTexture, vTexCoord));
    vec3 diffuse = light.diffuse * diff * vec3(texture(ourTexture, vTexCoord));
    vec3 specular = light.specular * spec * vec3(texture(ourTexture, vTexCoord));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(int lightId, vec3 normal, vec3 viewDir, vec4 lPosition) {
    vec3 lightDir = normalize(-lights[lightId].direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 0.5);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // combine results
    vec3 ambient =  attenuation * intensity * lights[lightId].ambient; //* vec3(texture(material.diffuseMap, vTexCoord));
    vec3 diffuse =  attenuation * intensity * lights[lightId].diffuse * diff; //* vec3(texture(material.diffuseMap, vTexCoord));
    vec3 specular = attenuation * intensity * lights[lightId].specular * spec; //* vec3(texture(material.specularMap, vTexCoord));
    float isShadow = shadowCalculation(lPosition, lights[lightId].shadowMap);
    return (ambient + diffuse*isShadow + specular*isShadow);
}