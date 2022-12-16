#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT {
    vec2 fragTexCoord;
    vec3 centralPosition;
    float time;
} gs_in[];

out GS_OUT {
    vec2 fragTexCoord;
} gs_out;

uniform sampler2D texture0;

void draw_particle(vec4 position, vec4 x, vec4 y) {
    gl_Position = position + .1*x;
    gs_out.fragTexCoord = gs_in[0].fragTexCoord;
    EmitVertex();
    gl_Position = position + .1*y;
    gs_out.fragTexCoord = gs_in[0].fragTexCoord;
    EmitVertex();
    gl_Position = position + .1*x + .1*y;
    gs_out.fragTexCoord = gs_in[0].fragTexCoord;
    EmitVertex();
}

void main() {
    vec3 x = normalize(vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position));
    vec3 y = normalize(vec3(gl_in[1].gl_Position) - vec3(gl_in[2].gl_Position) - x);

    // get speed from perlin noise texture
    ivec2 pixelCoord = ivec2(gs_in[0].fragTexCoord * 200.);
    vec4 pixelColor = texelFetch(texture0, pixelCoord, 0);

    // some funny animation
    vec3 dest = normalize(vec3(gl_in[0].gl_Position) - gs_in[0].centralPosition);
    vec3 displacement = dest * (pixelColor.x - .2)
        * sin(10. * pixelColor.x * gs_in[0].time + pixelColor.x);

    draw_particle(gl_in[0].gl_Position + vec4(displacement, .0), vec4(x,.0), vec4(y,.0));
    EndPrimitive();
}
