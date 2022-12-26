#version 330 core

layout (location = 0) in vec3 position;
in vec2 texCoord;
out vec2 vTexCoord;

void main() {
    gl_Position = vec4(position.xy, 0.f, 1.f);
    vTexCoord = texCoord;
}