#ifndef OPENGL_TEST2_CUBEPREFAB_H
#define OPENGL_TEST2_CUBEPREFAB_H

#include "engine/Renderer.h"
#include "components/Renderable.h"
#include <entt/entt.hpp>

namespace prefabs {

void cubePrefab(engine::RenderAssetRef asset, entt::registry &registry,
                glm::vec3 position);
engine::RenderAssetRef cubePrefabLoader(engine::Renderer &renderer);

}

#endif // OPENGL_TEST2_CUBEPREFAB_H
