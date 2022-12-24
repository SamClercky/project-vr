#version 330 core

out vec4 FragColor;
in vec3 vPosition;
in vec2 vTexCoord;
in vec3 vNormal;

//uniform vec3 lightColor = vec3(1.0f);
uniform sampler2D ourTexture;

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
};

uniform vec3 viewPos;
//uniform Material material;
uniform int numLights;
uniform Light lights[2]; // -> temp fix for the invisible objects bug..

vec3 CalcPointLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main() {
	vec3 norm = normalize(vNormal);
	vec3 viewDir = normalize(viewPos - vPosition);

    vec3 result = vec3(0.0f);
    for (int i=0; i<numLights; i++) {
        Light l = lights[i];
        if (l.cutOff == 0.0) {
            result += CalcPointLight(l, norm, vPosition, viewDir);
        } else {
            result += CalcSpotLight(l, norm, vPosition, viewDir);
        }    
    }
	//FragColor = texture(ourTexture, vTexCoord) * vec4(1.0,0.0,0.0, 1.0);
    FragColor = vec4(result,1.0);
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

vec3 CalcSpotLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
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
    // used color vec ins.of texture because not all objects have textures yet
    vec3 ambient = light.ambient * vec3(0.33,0.78,0.86);
    vec3 diffuse = light.diffuse * diff * vec3(0.33,0.78,0.86);
    vec3 specular = light.specular * spec * vec3(0.33,0.78,0.86);
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}