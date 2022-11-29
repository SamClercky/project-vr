#ifndef OPENGL_TEST2_SCENE_H
#define OPENGL_TEST2_SCENE_H

#include "engine/Renderer.h"
#include <entt/entt.hpp>

namespace scenes {

class Scene {
public:
  explicit Scene(engine::Renderer &renderer);
  ~Scene() = default;

  void update(uint64_t deltaTime);
  void render(engine::Renderer::RenderGuard &renderer);

private:
  entt::registry m_registry;
};

}

#endif // OPENGL_TEST2_SCENE_H
