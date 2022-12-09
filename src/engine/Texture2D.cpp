#include "Texture2D.h"
#include "stb_image.h"
#include <iostream>

using namespace engine;

Texture2D::Texture2D() {
    textureID = 0;
    glGenTextures(1, &textureID);
}

Texture2D::Texture2D(const char *path) {
    textureID = 0;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    int width, height, nrChannels;
    uint8_t *data = stbi_load(path, &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                     GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cerr << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture2D::~Texture2D() = default;

void Texture2D::configure_texture(TextureConfig config) const {
    glBindTexture(GL_TEXTURE_2D, textureID);

    if (config.texture_wrap_s != GLTextureRepeat::IGNORE)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                        static_cast<GLint>(config.texture_wrap_s));
    if (config.texture_wrap_t != GLTextureRepeat::IGNORE)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
                        static_cast<GLint>(config.texture_wrap_t));
    if (config.texture_mag_filter != GLFilter::IGNORE)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                        static_cast<GLint>(config.texture_mag_filter));
    if (config.texture_min_filter != GLFilter::IGNORE)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                        static_cast<GLint>(config.texture_min_filter));

    glBindTexture(GL_TEXTURE_2D, 0);
}
