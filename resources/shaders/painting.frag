#version 330 core

out vec4 FragColor;

in vec2 vTexCoord;

uniform sampler2D paintingTexture;

void main() {
    FragColor = texture(paintingTexture, vTexCoord);
}