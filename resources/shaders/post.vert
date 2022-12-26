#version 330 core

layout (location = 0) in vec3 position;
in vec2 texCoord;

out VS_OUT {
    vec2 texCoord;
    vec2 position;
} vs_out;

void main() {
    gl_Position = vec4(position.xy, 0.f, 1.f);
    vs_out.texCoord = texCoord;
    vs_out.position = position.xy;
}