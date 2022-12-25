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
    vec3 worldPosition;
    vec3 normal;
    vec2 texCoord;
    vec3 viewPosition;
    vec4 lightPositions[16];
} fs_in;

uniform vec3 lightColor = vec3(1.0f);
uniform sampler2D ourTexture;

//uniform Material material;
uniform Light lights[16];
uniform int numLights;

float shadowCalculation(vec4 lPosition, in sampler2D shadowMap);
vec3 CalcPointLight(int lightId, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(int lightId, vec3 normal, vec4 lPosition, vec3 fragPos, vec3 viewDir);

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
        if (lights[i].cutOff == 0.0) {
            result += CalcPointLight(i, norm, fs_in.worldPosition, viewDir);
        } else {
            result += CalcSpotLight(i, norm, fs_in.lightPositions[i], fs_in.worldPosition, viewDir);
        }
    }

    FragColor = texture(ourTexture, fs_in.texCoord) * vec4(result, 1.0);
}

vec3 CalcPointLight(int lightId, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(lights[lightId].position - fragPos);

    //diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    //specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 0.5); //shininess

    //attenuation
    float distance = length(lights[lightId].position - fragPos);
    float attenuation = 1.0 / (lights[lightId].constant + lights[lightId].linear*distance + lights[lightId].quadratic*(distance*distance));

    // combine
    vec3 ambient = lights[lightId].ambient * vec3(texture(ourTexture, fs_in.texCoord));
    vec3 diffuse = lights[lightId].diffuse * diff * vec3(texture(ourTexture, fs_in.texCoord));
    vec3 specular = lights[lightId].specular * spec * vec3(texture(ourTexture, fs_in.texCoord));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(int lightId, vec3 normal, vec4 lPosition, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(lights[lightId].position - fragPos);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 0.5);

    // attenuation
    float distance = length(lights[lightId].position - fragPos);
    float attenuation = 1.0 / (lights[lightId].constant + lights[lightId].linear * distance + lights[lightId].quadratic * (distance * distance));

    // spotlight intensity
    float theta = dot(lightDir, normalize(-lights[lightId].direction));
    float epsilon = lights[lightId].cutOff - lights[lightId].outerCutOff;
    float intensity = clamp((theta - lights[lightId].outerCutOff) / epsilon, 0.0, 1.0);
    intensity = 1.f;

    // combine results
    // used color vec ins.of texture because not all objects have textures yet
    vec3 ambient = lights[lightId].ambient;// * vec3(0.33,0.78,0.86);
    vec3 diffuse = lights[lightId].diffuse * diff;// * vec3(0.33,0.78,0.86);
    vec3 specular = lights[lightId].specular * spec;// * vec3(0.33,0.78,0.86);
    float shadow = shadowCalculation(lPosition, lights[lightId].shadowMap);

    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity * shadow;
    specular *= attenuation * intensity * shadow;
    return (ambient + diffuse + specular);
}
