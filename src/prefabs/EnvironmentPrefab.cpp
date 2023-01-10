#include "EnvironmentPrefab.h"

#include "components/Renderable.h"
#include "components/Position.h"
#include "components/CollisionObject.h"

void prefabs::environmentPrefab(entt::registry & registry, std::shared_ptr<engine::Shader> &prefabShader, std::shared_ptr<engine::Model> &model, std::unique_ptr<btDiscreteDynamicsWorld> &world) {
    const auto entity = registry.create();
    registry.emplace<components::Renderable>(entity, std::vector<std::shared_ptr<engine::Model>>{model}, prefabShader, false);

    const float thickness = 1.f;
    const float width = 24.f;
    const float height = 24.f;
    float wallHeight = 100.f;

    glm::vec3 position{0.f, 0.f, 0.f};
    auto &pos = registry.emplace<components::Position>(entity, glm::vec3{0.0}, 5.f);
    glm::vec3 size{width, thickness, height};

    glm::vec3 groundSize{2.f*width, thickness, 2.f*height};

    glm::vec3 wall1Size{thickness, wallHeight, height};
    glm::vec3 wall1Pos = glm::vec3{width/2.f, -10.f, 0.f};

    glm::vec3 wall2Size{thickness, wallHeight, height};
    glm::vec3 wall2Pos = glm::vec3{-width/2.f, -10.f, 0.f};

    glm::vec3 wall3Size{width, wallHeight, thickness};
    glm::vec3 wall3Pos = glm::vec3{0.f, -10.f, height/2.f};

    glm::vec3 wall4Size{width, wallHeight, thickness};
    glm::vec3 wall4Pos = glm::vec3{0.f, -10.f, -height/2.f};

    auto &cObject = registry.emplace<components::CollisionObject>(entity, components::collisionobject::cubeCompound(
            world, position, glm::mat3{1.f}, 0.f, std::vector<components::collisionobject::CubeShape>{
                    components::collisionobject::CubeShape{ .size=groundSize, .position=glm::vec3{0.f}, .orientation=glm::mat3{1.f}},
                    components::collisionobject::CubeShape{ .size=wall1Size, .position=wall1Pos, .orientation=glm::mat3{1.f}},
                    components::collisionobject::CubeShape{ .size=wall2Size, .position=wall2Pos, .orientation=glm::mat3{1.f}},
                    components::collisionobject::CubeShape{ .size=wall3Size, .position=wall3Pos, .orientation=glm::mat3{1.f}},
                    components::collisionobject::CubeShape{ .size=wall4Size, .position=wall4Pos, .orientation=glm::mat3{1.f}},
            }), world.get(), size);
    cObject.body->setSleepingThresholds(0.f, 0.f);
    cObject.body->setAngularFactor(0.f);
}
void prefabs::environmentPrefabLoader(std::shared_ptr<engine::Shader> &outShader, std::shared_ptr<engine::Model> &outModel) {
//    outShader = engine::GlobalAssetManager.loadShader(RESOURCES_ROOT / "shaders" / "rabbit.vert", RESOURCES_ROOT / "shaders" / "rabbit.frag");
    auto texture = engine::GlobalAssetManager.loadTexture(RESOURCES_ROOT / "room_bake.png");
    texture->configure_texture({
                                           .texture_wrap_s = engine::GLTextureRepeat::IGNORE,
                                           .texture_wrap_t = engine::GLTextureRepeat::IGNORE,
                                           .texture_min_filter = engine::GLFilter::LINEAR,
                                           .texture_mag_filter = engine::GLFilter::LINEAR,
                                   });
    outShader = engine::GlobalAssetManager.loadShader(RESOURCES_ROOT / "shaders" / "light.vert", RESOURCES_ROOT / "shaders" / "light.frag");
    outModel = engine::GlobalAssetManager.loadModel(RESOURCES_ROOT / "3dobj" / "room.obj", outShader);
    outModel->meshes[0]->attachTexture(texture);
}
