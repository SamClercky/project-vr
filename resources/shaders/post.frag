#version 330 core

out vec4 FragColor;
in vec2 vTexCoord;
uniform sampler2D screenTexture;

void main() {
    FragColor = texture(screenTexture, vTexCoord);
}