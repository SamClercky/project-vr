#version 330 core

out vec4 gl_Position;

layout (location = 0) in vec3 position;
in vec3 normal;
in vec2 texCoord;

out VS_OUT {
	vec3 position;
	vec3 worldPosition;
	vec3 normal;
	vec2 texCoord;
	vec3 viewPosition;
	vec4 lightPositions[16];
} vs_out;

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

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 viewPosition;

uniform Light lights[16];
uniform int numLights;

void main() {
	gl_Position = projection * view * model * vec4(position, 1.0);
	vs_out.position = position;
	vs_out.worldPosition = vec3(model * vec4(position, 1.f));
	vs_out.normal = transpose(inverse(mat3(model))) * normal;
	vs_out.texCoord = texCoord;
	vs_out.viewPosition = viewPosition;

	for (int i = 0; i < numLights && i < 16; i++) {
		vs_out.lightPositions[i] = lights[i].lightTransform * model * vec4(position, 1.f);
	}
}