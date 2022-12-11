#ifndef PROJECT_VR_CUBEMAPPREFAB_H
#define PROJECT_VR_CUBEMAPPREFAB_H

#include "engine/Model.h"
#include <memory>
#include <entt/entt.hpp>

namespace prefabs {
    void cubeMapPrefab(std::shared_ptr<engine::Model> &asset,
                    std::shared_ptr<engine::Shader> &shader,
                    entt::registry &registry);
    void cubeMapPrefabLoader(std::shared_ptr<engine::Model> &outModel,
                          std::shared_ptr<engine::Shader> &outShader);
}

#endif//PROJECT_VR_CUBEMAPPREFAB_H
