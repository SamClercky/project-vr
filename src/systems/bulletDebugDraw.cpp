#include "bulletDebugDraw.h"
#include "BulletCollision/CollisionShapes/btBoxShape.h"
#include "components/CollisionObject.h"
#include "components/DeltaTime.h"
#include "engine/AssetManager.h"
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

auto bulletBoundingBox = std::vector{
        engine::Vertex{ -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, },
        engine::Vertex{ 0.5f, -0.5f, -0.5f, 1.0f, 0.0f, },
        engine::Vertex{ 0.5f, 0.5f, -0.5f, 1.0f, 1.0f, },
        engine::Vertex{ 0.5f, 0.5f, -0.5f, 1.0f, 1.0f, },
        engine::Vertex{ -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, },
        engine::Vertex{ -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, },
        engine::Vertex{ -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, },
        engine::Vertex{ 0.5f, -0.5f, 0.5f, 1.0f, 0.0f, },
        engine::Vertex{ 0.5f, 0.5f, 0.5f, 1.0f, 1.0f, },
        engine::Vertex{ 0.5f, 0.5f, 0.5f, 1.0f, 1.0f, },
        engine::Vertex{ -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, },
        engine::Vertex{ -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, },
        engine::Vertex{ -0.5f, 0.5f, 0.5f, 1.0f, 0.0f, },
        engine::Vertex{ -0.5f, 0.5f, -0.5f, 1.0f, 1.0f, },
        engine::Vertex{ -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, },
        engine::Vertex{ -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, },
        engine::Vertex{ -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, },
        engine::Vertex{ -0.5f, 0.5f, 0.5f, 1.0f, 0.0f, },
        engine::Vertex{ 0.5f, 0.5f, 0.5f, 1.0f, 0.0f, },
        engine::Vertex{ 0.5f, 0.5f, -0.5f, 1.0f, 1.0f, },
        engine::Vertex{ 0.5f, -0.5f, -0.5f, 0.0f, 1.0f, },
        engine::Vertex{ 0.5f, -0.5f, -0.5f, 0.0f, 1.0f, },
        engine::Vertex{ 0.5f, -0.5f, 0.5f, 0.0f, 0.0f, },
        engine::Vertex{ 0.5f, 0.5f, 0.5f, 1.0f, 0.0f, },
        engine::Vertex{ -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, },
        engine::Vertex{ 0.5f, -0.5f, -0.5f, 1.0f, 1.0f, },
        engine::Vertex{ 0.5f, -0.5f, 0.5f, 1.0f, 0.0f, },
        engine::Vertex{ 0.5f, -0.5f, 0.5f, 1.0f, 0.0f, },
        engine::Vertex{ -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, },
        engine::Vertex{ -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, },
        engine::Vertex{ -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, },
        engine::Vertex{ 0.5f, 0.5f, -0.5f, 1.0f, 1.0f, },
        engine::Vertex{ 0.5f, 0.5f, 0.5f, 1.0f, 0.0f, },
        engine::Vertex{ 0.5f, 0.5f, 0.5f, 1.0f, 0.0f, },
        engine::Vertex{ -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, },
        engine::Vertex{-0.5f, 0.5f, -0.5f, 0.0f, 1.0f}};

std::shared_ptr<engine::Shader> shader;
std::shared_ptr<engine::Mesh> mesh;
std::shared_ptr<engine::Mesh> meshCircle;

void systems::bulletDebugDrawSystem(entt::registry &registry, engine::Renderer::RenderGuard &guard, std::unique_ptr<btDiscreteDynamicsWorld> &world) {
    auto &dt = registry.ctx().get<components::DeltaTime>();
    const auto &view = registry.view<components::CollisionObject>();
    for (const auto &entity: view) {
        auto &collisionObject = view.get<components::CollisionObject>(entity);
        const auto &shape = collisionObject.body->getCollisionShape();

        btTransform transform;
        collisionObject.body->getMotionState()->getWorldTransform(transform);
        auto orientation = glm::mat3_cast(glm::qua{
                transform.getRotation().getW(),
                transform.getRotation().getX(),
                transform.getRotation().getY(),
                transform.getRotation().getZ()});

        orientation[0] *= collisionObject.size.x;
        orientation[1] *= collisionObject.size.y;
        orientation[2] *= collisionObject.size.z;

        glm::vec3 translation{
                transform.getOrigin().getX() - collisionObject.offset.x,
                transform.getOrigin().getY() - collisionObject.offset.y,
                transform.getOrigin().getZ() - collisionObject.offset.z,
        };
        glm::mat4 modelView{
            glm::vec4{orientation[0], translation.x},
            glm::vec4{orientation[1], translation.y},
            glm::vec4{orientation[2], translation.z},
            glm::vec4{0.f, 0.f, 0.f, 1.f},
        };

        btVector3 center;
        btScalar radius;
        shape->getBoundingSphere(center, radius);
        glm::mat4 circleBoundingBoxView{
                radius, 0.f, 0.f, transform.getOrigin().getX() + center.getX(),
                0.f, radius, 0.f, transform.getOrigin().getY() + center.getY(),
                0.f, 0.f, radius, transform.getOrigin().getZ() + center.getZ(),
                0.f, 0.f, 0.f, 1.f
        };

        if (!shader)
            shader = engine::GlobalAssetManager.loadShader(RESOURCES_ROOT / "shaders" / "v_shader.glsl",
                                                                RESOURCES_ROOT / "shaders" / "bullet_debug.geom",
                                                                RESOURCES_ROOT / "shaders" / "bullet_debug.frag");
        if (!mesh) {
            mesh = std::make_shared<engine::Mesh>(
                    std::move(bulletBoundingBox),
                    std::vector<uint32_t>{},
                    std::vector<std::shared_ptr<engine::Texture2D>>{},
                    shader);
            engine::GlobalAssetManager.submitMesh(RESOURCES_SRC_ROOT / "systems" / "bulletDebugDraw.cpp", mesh);
        }
        if (!meshCircle) {
            auto model = engine::GlobalAssetManager.loadModel(RESOURCES_ROOT / "3dobj" / "sphere_low_poly.obj", shader);
            meshCircle = model->meshes[0];
        }

        guard.submit(engine::RenderAssetRef {
                .mesh = mesh,
                .shader = shader,
        }, glm::transpose(modelView));
        guard.submit(engine::RenderAssetRef {
                .mesh = meshCircle,
                .shader = shader,
        }, glm::transpose(circleBoundingBoxView));
    }
}
