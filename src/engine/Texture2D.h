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
  IGNORE = 0, // no set
  LINEAR = GL_LINEAR,
  NEAREST = GL_NEAREST,
};

enum class TextureLocation {
  Texture0 = GL_TEXTURE0,
  Texture1 = GL_TEXTURE1,
  Texture2 = GL_TEXTURE2,
  Texture3 = GL_TEXTURE3,
  Texture4 = GL_TEXTURE4,
  Texture5 = GL_TEXTURE5,
  Texture6 = GL_TEXTURE6,
  Texture7 = GL_TEXTURE7,
  Texture8 = GL_TEXTURE8,
  Texture9 = GL_TEXTURE9,
  Texture10 = GL_TEXTURE10,
  Texture11 = GL_TEXTURE11,
  Texture12 = GL_TEXTURE12,
  Texture13 = GL_TEXTURE13,
  Texture14 = GL_TEXTURE14,
  Texture15 = GL_TEXTURE15,
};

struct TextureConfig {
  GLTextureRepeat texture_wrap_s;
  GLTextureRepeat texture_wrap_t;
  GLFilter texture_min_filter;
  GLFilter texture_mag_filter;
};

struct BoundedTexture2DGuard {
  uint32_t textureID;
  TextureLocation location;

  BoundedTexture2DGuard(uint32_t textureID, TextureLocation location)
      : textureID(textureID), location(location) {
    glActiveTexture(static_cast<GLenum>(location));
    glBindTexture(GL_TEXTURE_2D, textureID);
  }
  ~BoundedTexture2DGuard() {
    glActiveTexture(static_cast<GLenum>(location));
    glBindTexture(GL_TEXTURE_2D, textureID);
  }
};

class Texture2D {
public:
  Texture2D();
  explicit Texture2D(const char *path);
  ~Texture2D();
  Texture2D(Texture2D &&other) = default;

  void configure_texture(TextureConfig config) const;
  [[nodiscard]] BoundedTexture2DGuard bind(TextureLocation location) const {
    return {textureID, location};
  }

private:
  uint32_t textureID;
};

}

#endif // OPENGL_TEST2_TEXTURE2D_H
