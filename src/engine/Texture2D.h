#ifndef OPENGL_TEST2_TEXTURE2D_H
#define OPENGL_TEST2_TEXTURE2D_H

#include "glad/glad.h"
#include <cstdint>
#include <filesystem>

namespace engine {

    enum class GLTextureRepeat {
        IGNORE = 0,
        Repeat = GL_REPEAT,
        MirroredRepeat = GL_MIRRORED_REPEAT,
        ClampToEdge = GL_CLAMP_TO_EDGE,
    };

    enum class GLFilter {
        IGNORE = 0,// no set
        LINEAR = GL_LINEAR,
        NEAREST = GL_NEAREST,
    };

    enum class TextureType {
        Image, Specular, Diffuse, CubeMap,
    };

    struct TextureConfig {
        GLTextureRepeat texture_wrap_s;
        GLTextureRepeat texture_wrap_t;
        GLTextureRepeat texture_wrap_r;
        GLFilter texture_min_filter;
        GLFilter texture_mag_filter;
    };

    struct TextureCubeMapSide {
        uint8_t* data;
        int width;
        int height;
    };

    struct TextureCubeMap {
        TextureCubeMapSide front;
        TextureCubeMapSide back;
        TextureCubeMapSide left;
        TextureCubeMapSide right;
        TextureCubeMapSide top;
        TextureCubeMapSide bottom;
    };

    struct BoundedTexture2DGuard {
        uint32_t textureID;
        GLenum location;
        GLenum textureBind;
        bool engaged;

        // Creates unengaged guard
        BoundedTexture2DGuard()
                : textureID(0), location(0), engaged(false), textureBind(GL_TEXTURE_2D) {}

        BoundedTexture2DGuard(uint32_t textureID, uint8_t location, GLenum textureBind)
            : textureID(textureID), location(location), engaged(true), textureBind(textureBind) {
            if (location > 15) throw std::runtime_error("Texture out of range");
            this->location = GL_TEXTURE0 + location;
            glActiveTexture(this->location);
            glBindTexture(textureBind, textureID);
        }
        ~BoundedTexture2DGuard() noexcept {
            if (!engaged) return;
            glActiveTexture(location);
            glBindTexture(textureBind, 0);
        }

        BoundedTexture2DGuard(BoundedTexture2DGuard &&other) noexcept {
            this->textureID = other.textureID;
            this->location = other.location;
            this->textureBind = other.textureBind;
            other.engaged = false;
        }

        BoundedTexture2DGuard& operator =(BoundedTexture2DGuard &&other) noexcept {
            this->textureID = other.textureID;
            this->location = other.location;
            this->textureBind = other.textureBind;
            this->engaged = true;
            other.engaged = false;

            return *this;
        }
    };

    class Texture2D {
    public:
        Texture2D();
        explicit Texture2D(uint8_t const *data, int width, int height, int nrChannels, TextureType type);
        explicit Texture2D(TextureCubeMap data);
        ~Texture2D();
        Texture2D(Texture2D const &other) = default;
        Texture2D(Texture2D &&other) noexcept {
            m_textureID = other.m_textureID;
            m_textureBind = other.m_textureBind;
            other.m_textureID = 0;
            m_type = other.m_type;
        }

        void configure_texture(TextureConfig config) const;
        [[nodiscard]] BoundedTexture2DGuard bind(uint8_t location) const {
            return {m_textureID, location, m_textureBind};
        }

    private:
        uint32_t m_textureID;
        TextureType m_type;
        GLenum m_textureBind;
    };

}// namespace engine

#endif// OPENGL_TEST2_TEXTURE2D_H
