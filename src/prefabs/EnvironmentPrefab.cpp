#include "EnvironmentPrefab.h"

#include "components/Renderable.h"
#include "components/Position.h"
#include "components/CollisionObject.h"

void prefabs::environmentPrefab(entt::registry & registry, std::shared_ptr<engine::Shader> &prefabShader, std::shared_ptr<engine::Model> &model, std::unique_ptr<btDiscreteDynamicsWorld> &world) {
    const auto entity = registry.create();
    registry.emplace<components::Renderable>(entity, std::vector<std::shared_ptr<engine::Model>>{model}, prefabShader);
    glm::vec3 position{0.f, -1.f, 0.f};
    auto &pos = registry.emplace<components::Position>(entity, glm::vec3{0.0}, 5.f);
    glm::vec3 size{10.f, 0.f, 10.f};
    registry.emplace<components::CollisionObject>(entity, components::collisionobject::cube(
            world, size, position, glm::mat3{1.f}, 0.f), size);
}
void prefabs::environmentPrefabLoader(std::shared_ptr<engine::Shader> &outShader, std::shared_ptr<engine::Model> &outModel) {
    outShader = engine::GlobalAssetManager.loadShader(RESOURCES_ROOT / "shaders" / "rabbit.vert", RESOURCES_ROOT / "shaders" / "rabbit.frag");
    outModel = engine::GlobalAssetManager.loadModel(RESOURCES_ROOT / "3dobj" / "env.obj", outShader);
}
