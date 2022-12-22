#include "PlayerPrefab.h"
#include "components/Position.h"
#include "components/CollisionObject.h"
#include "components/FollowedByCamera.h"

void prefabs::playerPrefab(entt::registry &registry, std::unique_ptr<btDiscreteDynamicsWorld> &world) {
    const auto &entity = registry.create();
    glm::vec3 position{0.f, 2.f, 0.f};
    glm::vec3 size{.5f, 2.f, .5f};
    registry.emplace<components::Position>(entity, position);
    auto &cObject = registry.emplace<components::CollisionObject>(entity,
                                                  components::collisionobject::cube(world, size, position, glm::mat3{1.f}, 1.f),
                                                  world.get(), size);
    cObject.body->setSleepingThresholds(0.f, 0.f);
    cObject.body->setAngularFactor(0.f);

    registry.emplace<components::FollowedByCamera>(entity);
}