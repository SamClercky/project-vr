#include "EnvironmentPrefab.h"

#include "components/Renderable.h"
#include "components/Position.h"

void prefabs::environmentPrefab(entt::registry &registry, std::shared_ptr<engine::Shader> &prefabShader, std::shared_ptr<engine::Model> &model) {
    const auto entity = registry.create();
    registry.emplace<components::Renderable>(entity, std::vector<std::shared_ptr<engine::Model>>{model}, prefabShader);
    registry.emplace<components::Position>(entity, glm::vec3{0.0});
    registry.get<components::Position>(entity).scale(10.f);
}
void prefabs::environmentPrefabLoader(std::shared_ptr<engine::Shader> &outShader, std::shared_ptr<engine::Model> &outModel) {
    outShader = engine::GlobalAssetManager.loadShader(RESOURCES_ROOT / "shaders" / "rabbit.vert", RESOURCES_ROOT / "shaders" / "rabbit.frag");
    outModel = engine::GlobalAssetManager.loadModel(RESOURCES_ROOT / "3dobj" / "env.obj", outShader);
}
