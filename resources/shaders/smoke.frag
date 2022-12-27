#version 330 core

out vec4 FragColor;

in GS_OUT {
    vec2 fragTexCoord;
} fs_in;

uniform sampler2D texture0;
uniform float time;

void main() {
    float intensity = texture(texture0, fs_in.fragTexCoord).x;
    FragColor = vec4(intensity, intensity, intensity, 1. - intensity * abs(sin(time) + .2 * cos(2*time)));
}