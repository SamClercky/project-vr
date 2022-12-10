#version 330 core

out vec4 gl_Position;

in vec3 position;
out vec3 vPosition;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * model * (10.f*vec4(position, 1.0));
    vPosition = position;
}