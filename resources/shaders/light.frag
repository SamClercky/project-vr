#version 330 core

out vec4 FragColor;
in vec3 vPosition;
in vec2 vTexCoord;
in vec3 vNormal;

uniform vec3 lightColor = vec3(1.0f);
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
};

uniform vec3 viewPos;
//uniform Material material;
uniform Light lights[];
uniform int numLights;

vec3 CalcDirLight(Light light, vec3 normal, vec3 viewDir);

void main() {
	vec3 norm = normalize(vNormal);
	vec3 viewDir = normalize(viewPos - vPosition);

	//phase 1: directional lighting
    vec3 result = vec3(0.0f);
    for (int i=0; i<numLights; i++) {
        Light l = lights[i];
        result += CalcDirLight(l, norm, viewDir);
    }

	FragColor = texture(ourTexture, vTexCoord) * vec4(1.0);
}

vec3 CalcDirLight(Light light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 0.5f);//shine
    // combine results
    vec3 ambient = light.ambient; //* vec3(texture(material.diffuseMap, vTexCoord));
    vec3 diffuse = light.diffuse * diff; //* vec3(texture(material.diffuseMap, vTexCoord));
    vec3 specular = light.specular * spec; //* vec3(texture(material.specularMap, vTexCoord));
    return (ambient + diffuse + specular);
}