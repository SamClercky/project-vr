#include "ThrowableBulletPrefab.h"

#include "components/Renderable.h"
#include "components/Position.h"
#include "components/CollisionObject.h"

#include "components/ShortLivedObject.h"
#include "engine/AssetManager.h"

void prefabs::loadThrowableBulletPrefab(std::shared_ptr<engine::Model> &outModel, std::shared_ptr<engine::Shader> &outShader) {
    outShader = engine::GlobalAssetManager.loadShader(
            RESOURCES_ROOT / "shaders" / "rabbit.vert",
            RESOURCES_ROOT / "shaders" / "rabbit.frag"
            );
    outModel = engine::GlobalAssetManager.loadModel(
            RESOURCES_ROOT / "3dobj" / "sphere_smooth.obj",
            outShader
            );
}
void prefabs::throwableBulletPrefab(entt::registry &registry,
                                    std::unique_ptr<btDiscreteDynamicsWorld> &world,
                                    std::shared_ptr<engine::Model> &model,
                                    std::shared_ptr<engine::Shader> &shader,
                                    glm::vec3 position,
                                    glm::vec3 impulse) {
    const auto entity = registry.create();
    registry.emplace<components::Position>(entity, position, .1f);
    glm::vec3 size{0.2f};
    const auto &cObject = registry.emplace<components::CollisionObject>(entity, components::collisionobject::cube(
                                                                  world,
                                                                  size,
                                                                  position,
                                                                  glm::mat3{1.f},
                                                                  0.2f), world.get(), size);
    cObject.applyImpulse(impulse);
    registry.emplace<components::Renderable>(entity, std::vector<std::shared_ptr<engine::Model>>{model}, shader);
    registry.emplace<components::ShortLivedObject>(entity, 10.f);
}
