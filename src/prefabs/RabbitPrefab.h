#ifndef PROJECT_VR_RABBITPREFAB_H
#define PROJECT_VR_RABBITPREFAB_H

#include "engine/Model.h"
#include <entt/entt.hpp>

namespace prefabs {
    void rabbitPrefab(std::shared_ptr<engine::Model> &asset,
                      std::shared_ptr<engine::Shader> &shader,
                      entt::registry &registry,
                      glm::vec3 position);
    void rabbitPrefabLoader(std::shared_ptr<engine::Model> &outModel,
                            std::shared_ptr<engine::Shader> &outShader);
}

#endif//PROJECT_VR_RABBITPREFAB_H
