#ifndef PROJECT_VR_COLLISIONOBJECT_H
#define PROJECT_VR_COLLISIONOBJECT_H

#include "BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h"
#include "BulletDynamics/Dynamics/btRigidBody.h"
#include <cstdint>
#include <glm/glm.hpp>
#include <memory>
#include <vector>

namespace components {
    struct CollisionObject {
        btRigidBody *body;
        btDiscreteDynamicsWorld *world;

        glm::vec3 offset;

        CollisionObject(btRigidBody *body, btDiscreteDynamicsWorld *world): body(body), world(world), offset(glm::vec3{0.f}) {}
        CollisionObject(btRigidBody *body, btDiscreteDynamicsWorld *world, glm::vec3 offset): body(body), world(world), offset(offset) {}

        void applyImpulse(glm::vec3 impulse) const;
    };

    namespace collisionObject {
        struct CubeShape {
            glm::vec3 size;
            glm::vec3 position;
            glm::mat3 orientation;
        };
        btRigidBody* cube(
                std::unique_ptr<btDiscreteDynamicsWorld> &world,
                glm::vec3 size,
                glm::vec3 position,
                glm::mat3 orientation,
                float mass);

        btRigidBody* cubeKinematic(
                std::unique_ptr<btDiscreteDynamicsWorld> &world,
                glm::vec3 size,
                glm::vec3 position,
                glm::mat3 orientation,
                float mass);

        btRigidBody *cubeCompound(
                std::unique_ptr<btDiscreteDynamicsWorld> &world,
                glm::vec3 position,
                glm::mat3 orientation,
                float mass, const std::vector<CubeShape>& cubes);
    }
}

#endif//PROJECT_VR_COLLISIONOBJECT_H
