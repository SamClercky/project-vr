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
    };

    enum class GLFilter {
        IGNORE = 0,// no set
        LINEAR = GL_LINEAR,
        NEAREST = GL_NEAREST,
    };

    enum class TextureType {
        Image, Specular, Diffuse
    };

    struct TextureConfig {
        GLTextureRepeat texture_wrap_s;
        GLTextureRepeat texture_wrap_t;
        GLFilter texture_min_filter;
        GLFilter texture_mag_filter;
    };

    struct BoundedTexture2DGuard {
        uint32_t textureID;
        GLenum location;
        bool engaged;

        // Creates unengaged guard
        BoundedTexture2DGuard()
                : textureID(0), location(0), engaged(false) {}

        BoundedTexture2DGuard(uint32_t textureID, uint8_t location)
            : textureID(textureID), location(location), engaged(true) {
            if (location > 15) throw std::runtime_error("Texture out of range");
            this->location = GL_TEXTURE0 + location;
            glActiveTexture(this->location);
            glBindTexture(GL_TEXTURE_2D, textureID);
        }
        ~BoundedTexture2DGuard() noexcept {
            if (!engaged) return;
            glActiveTexture(location);
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        BoundedTexture2DGuard(BoundedTexture2DGuard &&other) noexcept {
            this->textureID = other.textureID;
            this->location = other.location;
            other.engaged = false;
        }

        BoundedTexture2DGuard& operator =(BoundedTexture2DGuard &&other) noexcept {
            this->textureID = other.textureID;
            this->location = other.location;
            this->engaged = true;
            other.engaged = false;

            return *this;
        }
    };

    class Texture2D {
    public:
        Texture2D();
        explicit Texture2D(uint8_t const *data, int width, int height, int nrChannels, TextureType type);
        ~Texture2D();
        Texture2D(Texture2D const &other) = default;
        Texture2D(Texture2D &&other) noexcept {
            textureID = other.textureID;
            other.textureID = 0;
            type = other.type;
        }

        void configure_texture(TextureConfig config) const;
        [[nodiscard]] BoundedTexture2DGuard bind(uint8_t location) const {
            return {textureID, location};
        }

    private:
        uint32_t textureID;
        TextureType type;
    };

}// namespace engine

#endif// OPENGL_TEST2_TEXTURE2D_H
