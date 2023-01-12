#include "CollisionObject.h"
#include "BulletCollision/CollisionShapes/btBoxShape.h"
#include "BulletCollision/CollisionShapes/btCollisionShape.h"
#include "BulletCollision/CollisionShapes/btCompoundShape.h"
#include "BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h"
#include "BulletDynamics/Dynamics/btRigidBody.h"
#include "LinearMath/btDefaultMotionState.h"
#include <glm/gtx/quaternion.hpp>
#include <vector>

btVector3 toBullet(glm::vec3 v);
btMatrix3x3 toBullet(glm::mat3 m);

btVector3 toBullet(glm::vec3 v) {
    return btVector3{v.x, v.y, v.z};
}

btMatrix3x3 toBullet(glm::mat3 m) {
    return btMatrix3x3 {
            toBullet(m[0]),
            toBullet(m[1]),
            toBullet(m[2]),
    };
}

btRigidBody* components::collisionobject::cubeKinematic(
        std::unique_ptr<btDiscreteDynamicsWorld> &world,
        glm::vec3 size,
        glm::vec3 position,
        glm::mat3 orientation,
        float mass) {
        auto body = cube(world, size, position, orientation, mass);
        body->setCollisionFlags(body->getFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
        body->setActivationState(DISABLE_DEACTIVATION);

        return body;
}

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

btRigidBody* components::collisionobject::cubeCompound(
        std::unique_ptr<btDiscreteDynamicsWorld> &world,
        glm::vec3 position,
        glm::mat3 orientation,
        float mass, const std::vector<CubeShape>& cubes) {
    // needs rescaling to match the rest of the engine (probably size measured as from origin)
//    btCollisionShape *shape = new btCompoundShape(btVector3(size.x, size.y, size.z) * .5f);
    auto *shape = new btCompoundShape(true, (int)cubes.size());
    for (auto cube: cubes)
        shape->addChildShape(
                btTransform{toBullet(cube.orientation), toBullet(cube.position)},
                new btBoxShape(btVector3(cube.size.x, cube.size.y, cube.size.z) * .5f));

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
