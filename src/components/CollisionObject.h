#ifndef PROJECT_VR_COLLISIONOBJECT_H
#define PROJECT_VR_COLLISIONOBJECT_H

#include "BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h"
#include "BulletDynamics/Dynamics/btRigidBody.h"
#include <cstdint>
#include <glm/glm.hpp>
#include <memory>

namespace components {
    struct CollisionObject {
        btRigidBody *body;

        CollisionObject(btRigidBody *body): body(body) {}
    };

    namespace collisionobject {
        btRigidBody* cube(
                std::unique_ptr<btDiscreteDynamicsWorld> &world,
                glm::vec3 size,
                glm::vec3 position,
                glm::mat3 orientation,
                float mass);
    }
}

#endif//PROJECT_VR_COLLISIONOBJECT_H
