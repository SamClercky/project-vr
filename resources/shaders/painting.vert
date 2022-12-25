#version 330 core

out vec4 gl_Position;

layout (location = 0) in vec3 position;

in vec2 texCoord;
out vec2 vTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * model * vec4(position, 1.f);
    vTexCoord = texCoord;
}