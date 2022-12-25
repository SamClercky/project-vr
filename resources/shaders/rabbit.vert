#version 330 core

out vec4 gl_Position;

layout (location = 0) in vec3 position;
out vec3 vPosition;

in vec2 texCoord;
out vec2 vTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float time;

void main() {
    gl_Position = projection * view * model * (10.f*vec4(position, 1.0));
    vPosition = position;
    vTexCoord = texCoord;
}