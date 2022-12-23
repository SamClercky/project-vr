#version 330 core

out vec4 gl_Position;

in vec3 position;
out vec3 vPosition;
in vec3 normal;
out vec3 vNormal;
in vec2 texCoord;
out vec2 vTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat4 lightSpaceMatrix;
out vec4 lPosition;

void main() {
	gl_Position = projection * view * model * vec4(position, 1.0);
	vPosition = position;
	vNormal = transpose(inverse(mat3(model))) * normal;
	vTexCoord = texCoord;
	lPosition = lightSpaceMatrix * vec4(vPosition, 1.f);
}