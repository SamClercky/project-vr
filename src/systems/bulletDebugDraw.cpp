#include "bulletDebugDraw.h"
#include "BulletCollision/CollisionShapes/btBoxShape.h"
#include "BulletCollision/CollisionShapes/btCompoundShape.h"
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

static std::shared_ptr<engine::Shader> shader;
static std::shared_ptr<engine::Mesh> mesh;
static std::shared_ptr<engine::Mesh> meshCircle;

void drawBox(const btBoxShape* shape,
             const std::shared_ptr<engine::Shader> &boxShader,
             const std::shared_ptr<engine::Mesh> &boxMesh,
             const glm::quat &orientation,
             const glm::vec3 &translation,
             engine::Renderer::RenderGuard &guard);

void drawBox(const btBoxShape* shape,
             const std::shared_ptr<engine::Shader> &boxShader,
             const std::shared_ptr<engine::Mesh> &boxMesh,
             const glm::quat &orientation,
             const glm::vec3 &translation,
             engine::Renderer::RenderGuard &guard) {
    const auto &boxSize = shape->getHalfExtentsWithoutMargin()*2.f; // 2x half size -> full size

    const auto m_orientation = glm::mat3_cast(orientation);
    glm::mat4 modelView{
            glm::vec4{m_orientation[0]*boxSize.getX(), translation.x},
            glm::vec4{m_orientation[1]*boxSize.getY(), translation.y},
            glm::vec4{m_orientation[2]*boxSize.getZ(), translation.z},
            glm::vec4{0.f, 0.f, 0.f, 1.f},
    };

    guard.submit(engine::RenderAssetRef {
            .mesh = boxMesh,
            .shader = boxShader,
    }, glm::transpose(modelView), false);
}

void systems::bulletDebugDrawSystem(entt::registry &registry, engine::Renderer::RenderGuard &guard, std::unique_ptr<btDiscreteDynamicsWorld> &world) {
    // init code
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

    const auto &view = registry.view<components::CollisionObject>();
    for (const auto &entity: view) {
        auto &collisionObject = view.get<components::CollisionObject>(entity);
        const auto* shape = collisionObject.body->getCollisionShape();

        btTransform transform;
        collisionObject.body->getMotionState()->getWorldTransform(transform);
        const glm::qua globalOrientation{
                transform.getRotation().getW(),
                transform.getRotation().getX(),
                transform.getRotation().getY(),
                transform.getRotation().getZ()
        };

        glm::vec3 translation{
                transform.getOrigin().getX(),
                transform.getOrigin().getY(),
                transform.getOrigin().getZ(),
        };

        if (const auto* boxShape = dynamic_cast<const btBoxShape*>(shape)) {
            drawBox(boxShape, shader, mesh, globalOrientation, translation, guard);
        } else if (const auto* compoundShape = dynamic_cast<const btCompoundShape*>(shape)) {
            // we only go 1 level deep as this atm is the only level we can go
            for (int i = 0; i < compoundShape->getNumChildShapes(); i++) {
                if (const auto* boxShapeChild = dynamic_cast<const btBoxShape*>(compoundShape->getChildShape(i))) {
                    const auto &localTransform = compoundShape->getChildTransform(i);
                    const glm::qua localOrientation{
                            localTransform.getRotation().getW(),
                            localTransform.getRotation().getX(),
                            localTransform.getRotation().getY(),
                            localTransform.getRotation().getZ()};
                    const auto orientation = glm::inverse(globalOrientation) * localOrientation * globalOrientation;

                    const auto globalTransform = translation + glm::vec3{
                                                                       localTransform.getOrigin().getX(),
                                                                       localTransform.getOrigin().getY(),
                                                                       localTransform.getOrigin().getZ()
                                                               };
                    drawBox(boxShapeChild, shader, mesh, orientation, globalTransform, guard);
                }
            }
        }

        btVector3 center;
        btScalar radius;
        shape->getBoundingSphere(center, radius);
        glm::mat4 circleBoundingBoxView{
                radius, 0.f, 0.f, transform.getOrigin().getX() + center.getX(),
                0.f, radius, 0.f, transform.getOrigin().getY() + center.getY(),
                0.f, 0.f, radius, transform.getOrigin().getZ() + center.getZ(),
                0.f, 0.f, 0.f, 1.f
        };
        guard.submit(engine::RenderAssetRef {
                .mesh = meshCircle,
                .shader = shader,
        }, glm::transpose(circleBoundingBoxView), false);
    }
}
