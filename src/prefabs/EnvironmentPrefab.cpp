#include "EnvironmentPrefab.h"

#include "components/Renderable.h"
#include "components/Position.h"
#include "components/CollisionObject.h"

void prefabs::environmentPrefab(entt::registry & registry, std::shared_ptr<engine::Shader> &prefabShader, std::shared_ptr<engine::Model> &model, std::unique_ptr<btDiscreteDynamicsWorld> &world) {
    const auto entity = registry.create();
    registry.emplace<components::Renderable>(entity, std::vector<std::shared_ptr<engine::Model>>{model}, prefabShader);
    glm::vec3 position{0.f, -1.f, 0.f};
    auto &pos = registry.emplace<components::Position>(entity, glm::vec3{0.0}, 5.f);
    glm::vec3 size{30.f, 0.f, 20.f};
    registry.emplace<components::CollisionObject>(entity, components::collisionobject::cube(
            world, size, position, glm::mat3{1.f}, 0.f), size);

    glm::vec3 wall1Size{0.f, 10.f, 20.f};
    glm::vec3 wall1Pos = position + glm::vec3{15.f, 5.f, 0.f};
    const auto wall1 = registry.create();
    registry.emplace<components::CollisionObject>(wall1, components::collisionobject::cube(
             world, wall1Size, wall1Pos, glm::mat3{1.f}, 0.f), wall1Size);

    glm::vec3 wall2Size{0.f, 10.f, 20.f};
    glm::vec3 wall2Pos = position + glm::vec3{-15.f, 5.f, 0.f};
    const auto wall2 = registry.create();
    registry.emplace<components::CollisionObject>(wall2, components::collisionobject::cube(
            world, wall2Size, wall2Pos, glm::mat3{1.f}, 0.f), wall2Size);

    glm::vec3 wall3Size{30.f, 10.f, 0.f};
    glm::vec3 wall3Pos = position + glm::vec3{0.f, 5.f, 10.f};
    const auto wall3 = registry.create();
    registry.emplace<components::CollisionObject>(wall3, components::collisionobject::cube(
            world, wall3Size, wall3Pos, glm::mat3{1.f}, 0.f), wall3Size);

    glm::vec3 wall4Size{30.f, 10.f, 0.f};
    glm::vec3 wall4Pos = position + glm::vec3{0.f, 5.f, -10.f};
    const auto wall4 = registry.create();
    registry.emplace<components::CollisionObject>(wall4, components::collisionobject::cube(
            world, wall4Size, wall4Pos, glm::mat3{1.f}, 0.f), wall4Size);
}
void prefabs::environmentPrefabLoader(std::shared_ptr<engine::Shader> &outShader, std::shared_ptr<engine::Model> &outModel) {
    outShader = engine::GlobalAssetManager.loadShader(RESOURCES_ROOT / "shaders" / "rabbit.vert", RESOURCES_ROOT / "shaders" / "rabbit.frag");
    outModel = engine::GlobalAssetManager.loadModel(RESOURCES_ROOT / "3dobj" / "env.obj", outShader);
}
