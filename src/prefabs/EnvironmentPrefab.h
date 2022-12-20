#ifndef PROJECT_VR_ENVIRONMENTPREFAB_H
#define PROJECT_VR_ENVIRONMENTPREFAB_H

#include "engine/AssetManager.h"
#include "engine/Shader.h"
#include <entt/entt.hpp>
#include <memory>

namespace prefabs {
    void environmentPrefabLoader(std::shared_ptr<engine::Shader> &outShader, std::shared_ptr<engine::Model> &ouModel);
    void environmentPrefab(entt::registry & registry, std::shared_ptr<engine::Shader> &prefabShader, std::shared_ptr<engine::Model> &model);
}

#endif//PROJECT_VR_ENVIRONMENTPREFAB_H
