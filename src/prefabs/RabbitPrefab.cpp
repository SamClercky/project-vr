#include "RabbitPrefab.h"
#include "engine/AssetManager.h"
#include "components/Renderable.h"
#include "components/RotateAnimation.h"
#include "components/Position.h"

void prefabs::rabbitPrefab(std::shared_ptr<engine::Model> &asset,
                           std::shared_ptr<engine::Shader> &shader,
                           entt::registry &registry,
                           glm::vec3 position) {
    auto rabbitEntity = registry.create();
    registry.emplace<components::Renderable>(
            rabbitEntity, std::vector<std::shared_ptr<engine::Model>>{std::move(asset)}, shader);
    registry.emplace<components::RotateAnimation>(rabbitEntity, 20.f);
    registry.emplace<components::Position>(rabbitEntity, position);
}
void prefabs::rabbitPrefabLoader(std::shared_ptr<engine::Model> &outModel,
                                 std::shared_ptr<engine::Shader> &outShader) {

    outShader = engine::GlobalAssetManager.loadShader(RESOURCES_ROOT / "shaders" / "light.vert",
                                                        RESOURCES_ROOT / "shaders" / "light.frag");
    outModel = engine::GlobalAssetManager.loadModel(RESOURCES_ROOT / "3dobj" / "bunny_small.obj", outShader);
}
