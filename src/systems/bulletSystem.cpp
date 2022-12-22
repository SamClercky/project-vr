#include "bulletSystem.h"
#include "components/CollisionObject.h"
#include "components/DeltaTime.h"
#include "components/Position.h"
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

void systems::bulletSystem(entt::registry &registry, std::unique_ptr<btDiscreteDynamicsWorld> &dynamicWorld) {
    auto &dt = registry.ctx().get<components::DeltaTime>();
    dynamicWorld->stepSimulation(dt.sec());

    const auto &view = registry.view<components::Position, components::CollisionObject>();
    for (const auto &entity: view) {
        auto &position = view.get<components::Position>(entity);
        auto &collisionObject = view.get<components::CollisionObject>(entity);

        if (!collisionObject.body->getMotionState()) { continue; }

        btTransform transform;
        collisionObject.body->getMotionState()->getWorldTransform(transform);
        auto quatOrientation = glm::qua{
                transform.getRotation().getW(),
                transform.getRotation().getX(),
                transform.getRotation().getY(),
                transform.getRotation().getZ()};
        auto orientation = glm::mat4_cast(quatOrientation);
        auto pos = glm::vec3{
                transform.getOrigin().getX(),
                transform.getOrigin().getY(),
                transform.getOrigin().getZ()};

        position.set_rotation(orientation);
        position.set_translation(pos);
    }
}
