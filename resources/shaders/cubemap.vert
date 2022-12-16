#version 330 core

in vec3 position;
in vec3 texCoords;
in vec3 normal;

out vec3 vTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float time;

void main() {
    vTexCoord = position;
    mat4 noRot = mat4(mat3(view));
    vec4 pos = projection * noRot * vec4(position, 1.f);
    gl_Position = pos.xyww; // set max depth
}