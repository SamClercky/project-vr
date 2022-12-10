#version 330 core

out vec4 FragColor;
in vec3 vPosition;
in vec3 vTexCoord;

void main() {
    FragColor = vec4(vTexCoord, 1.f);
}