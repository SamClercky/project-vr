#version 330 core

out vec4 FragColor;
in vec3 vPosition;
in vec2 vTexCoord;

void main() {
    FragColor = vec4(vTexCoord, 0.f, 1.f);
}