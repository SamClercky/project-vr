#version 330 core

layout (location = 0) in vec3 position;
in vec3 normal;
in vec2 texCoord;

out VS_OUT {
    vec2 fragTexCoord;
    vec3 centralPosition;
    float time;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float time;

void main() {
    gl_Position = projection * view * model * vec4(position, 1.0);
    vs_out.fragTexCoord = texCoord;
    vs_out.centralPosition = vec3(model[3]);
    vs_out.time = time;
}