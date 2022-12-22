#ifndef PROJECT_VR_COLLISIONOBJECT_H
#define PROJECT_VR_COLLISIONOBJECT_H

#include "BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h"
#include "BulletDynamics/Dynamics/btRigidBody.h"
#include <cstdint>
#include <glm/glm.hpp>
#include <memory>
#include <glm/glm.hpp>

namespace components {
    struct CollisionObject {
        btRigidBody *body;
        glm::vec3 offset;
        glm::vec3 size;

        CollisionObject(btRigidBody *body, glm::vec3 size): body(body), size(size), offset(glm::vec3{0.f}) {}
        CollisionObject(btRigidBody *body, glm::vec3 size, glm::vec3 offset): body(body), size(size), offset(offset) {}
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
