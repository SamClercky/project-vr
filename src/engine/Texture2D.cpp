#include "Texture2D.h"
#include "stb_image.h"
#include <iostream>

using namespace engine;

Texture2D::Texture2D() : m_type(TextureType::Image), m_textureBind (GL_TEXTURE_2D) {
    m_textureID = 0;
    glGenTextures(1, &m_textureID);
}

Texture2D::Texture2D(uint8_t const *data, int width, int height, int nrChannels, TextureType type)
    : m_type(type), m_textureBind(GL_TEXTURE_2D) {
    m_textureID = 0;
    glGenTextures(1, &m_textureID);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(m_textureBind, m_textureID);

    glTexImage2D(m_textureBind, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(m_textureBind);

    glBindTexture(m_textureBind, 0);
}

Texture2D::Texture2D(TextureCubeMap data): m_type(TextureType::CubeMap), m_textureBind(GL_TEXTURE_CUBE_MAP) {
    m_textureID = 0;
    glGenTextures(1, &m_textureID);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(m_textureBind, m_textureID);

    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, data.front.width, data.front.height, 0, GL_RGB, GL_UNSIGNED_BYTE, data.front.data);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, data.back.width, data.back.height, 0, GL_RGB, GL_UNSIGNED_BYTE, data.back.data);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, data.left.width, data.left.height, 0, GL_RGB, GL_UNSIGNED_BYTE, data.left.data);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, data.right.width, data.right.height, 0, GL_RGB, GL_UNSIGNED_BYTE, data.right.data);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, data.top.width, data.top.height, 0, GL_RGB, GL_UNSIGNED_BYTE, data.top.data);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, data.bottom.width, data.bottom.height, 0, GL_RGB, GL_UNSIGNED_BYTE, data.bottom.data);

    glBindTexture(m_textureBind, 0);
}

Texture2D::~Texture2D() = default;

void Texture2D::configure_texture(TextureConfig config) const {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(m_textureBind, m_textureID);

    if (config.texture_wrap_s != GLTextureRepeat::IGNORE)
        glTexParameteri(m_textureBind, GL_TEXTURE_WRAP_S,
                        static_cast<GLint>(config.texture_wrap_s));
    if (config.texture_wrap_t != GLTextureRepeat::IGNORE)
        glTexParameteri(m_textureBind, GL_TEXTURE_WRAP_T,
                        static_cast<GLint>(config.texture_wrap_t));
    if (config.texture_wrap_r != GLTextureRepeat::IGNORE)
        glTexParameteri(m_textureBind, GL_TEXTURE_WRAP_R,
                        static_cast<GLint>(config.texture_wrap_r));
    if (config.texture_mag_filter != GLFilter::IGNORE)
        glTexParameteri(m_textureBind, GL_TEXTURE_MAG_FILTER,
                        static_cast<GLint>(config.texture_mag_filter));
    if (config.texture_min_filter != GLFilter::IGNORE)
        glTexParameteri(m_textureBind, GL_TEXTURE_MIN_FILTER,
                        static_cast<GLint>(config.texture_min_filter));

    glBindTexture(m_textureBind, 0);
}
