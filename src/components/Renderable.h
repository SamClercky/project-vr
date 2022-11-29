#ifndef OPENGL_TEST2_RENDERABLE_H
#define OPENGL_TEST2_RENDERABLE_H

#include "engine/Renderer.h"
#include <vector>

namespace components {

struct Renderable {
  std::vector<engine::RenderAssetRef> parts;

  explicit Renderable(std::vector<engine::RenderAssetRef> &&parts) : parts(parts) {}
  explicit Renderable(engine::RenderAssetRef &&part)
      : parts(std::vector<engine::RenderAssetRef>{part}) {}
};

} // namespace components

#endif // OPENGL_TEST2_RENDERABLE_H
