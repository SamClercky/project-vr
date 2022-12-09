#ifndef OPENGL_TEST2_CUBEPREFAB_H
#define OPENGL_TEST2_CUBEPREFAB_H

#include "components/Renderable.h"
#include "engine/Renderer.h"
#include <entt/entt.hpp>

namespace prefabs {

    void cubePrefab(engine::RenderAssetRef asset, entt::registry &registry,
                    glm::vec3 position);
    engine::RenderAssetRef cubePrefabLoader(engine::Renderer &renderer);

}// namespace prefabs

#endif// OPENGL_TEST2_CUBEPREFAB_H
