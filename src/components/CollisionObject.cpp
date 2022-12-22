#include "CollisionObject.h"
#include "BulletCollision/CollisionShapes/btBoxShape.h"
#include "BulletCollision/CollisionShapes/btCollisionShape.h"
#include "BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h"
#include "BulletDynamics/Dynamics/btRigidBody.h"
#include "LinearMath/btDefaultMotionState.h"
#include <glm/gtx/quaternion.hpp>


btRigidBody* components::collisionobject::cube(
        std::unique_ptr<btDiscreteDynamicsWorld> &world,
        glm::vec3 size,
        glm::vec3 position,
        glm::mat3 orientation,
        float mass) {
    // needs rescaling to match the rest of the engine (probably size measured as from origin)
    btCollisionShape *shape = new btBoxShape(btVector3(size.x, size.y, size.z) * .5f);
    auto quat = glm::quat_cast(orientation);
    auto *motionState = new btDefaultMotionState(btTransform(
            btQuaternion(quat.x, quat.y, quat.z, quat.w),
            btVector3(position.x, position.y, position.z)
            ));
    btRigidBody::btRigidBodyConstructionInfo rigidBodyInfo{
            mass,
            motionState,
            shape,
            btVector3{0.f, 0.f, 0.f}
    };
    auto *rigidBody = new btRigidBody(rigidBodyInfo);
    world->addRigidBody(rigidBody);

    return rigidBody;
}
void components::CollisionObject::applyImpulse(glm::vec3 impulse) const {
    body->applyCentralImpulse(btVector3{impulse.x, impulse.y, impulse.z});
}
