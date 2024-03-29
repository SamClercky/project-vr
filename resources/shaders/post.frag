#version 330 core

out vec4 FragColor;

in VS_OUT {
    vec2 texCoord;
    vec2 position;
} fs_in;

uniform sampler2DMS screenTexture;
uniform float time;
uniform float screenWidth;
uniform float screenHeight;

vec4 getAAPixel(vec2 texCoord) {
    vec2 screenSize = vec2(screenWidth, screenHeight);

    vec4 color = vec4(0.);
    for (int i = 0; i < 4; i++)
        color += texelFetch(screenTexture, ivec2(texCoord*screenSize), i);
    color /= 4.f;

    return color;
}

void main() {
    vec2 screenSize = vec2(screenWidth, screenHeight);
    vec2 uv = fs_in.position * screenSize/2.f;

    mat3 kernel = mat3(
        2.f,   2.f, 2.f,
        2.f, -15.f, 2.f,
        2.f,   2.f, 2.f
    );

    vec4 color = vec4(0.f);
    vec2 texelSize = 1.f / screenSize;
    for (int x = -1; x <= 1; x++) {
        for (int y = -1; y <= 1; y++) {
            vec4 pixel = getAAPixel(fs_in.texCoord + vec2(x, y) * texelSize);
            color += pixel * kernel[x+1][y+1];
        }
    }

    vec4 normalColor = getAAPixel(fs_in.texCoord);
    vec4 screenColor = mix(color, normalColor, abs(sin(.1*time)));

    if ((-50.f < uv.x && uv.x < 50.f && -1.f < uv.y && uv.y < 1.f) ||
        (-50.f < uv.y && uv.y < 50.f && -1.f < uv.x && uv.x < 1.f)) {
        FragColor = vec4(vec3(1.f) - screenColor.rgb, 1.f); // invert color
    } else {
        FragColor = screenColor;
    }
}