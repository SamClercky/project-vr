#version 330 core

out vec4 FragColor;

uniform samplerCube cubeSampler;
in vec3 vTexCoord;

void main() {
    FragColor = texture(cubeSampler, vTexCoord);
}