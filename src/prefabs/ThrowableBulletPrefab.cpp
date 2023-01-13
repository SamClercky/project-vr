#include "ThrowableBulletPrefab.h"

#include "components/Renderable.h"
#include "components/Position.h"
#include "components/CollisionObject.h"

#include "components/ShortLivedObject.h"
#include "engine/AssetManager.h"

void prefabs::loadThrowableBulletPrefab(std::shared_ptr<engine::Model> &outModel, std::shared_ptr<engine::Shader> &outShader) {
    auto bulletTexture = engine::GlobalAssetManager.loadTexture(RESOURCES_ROOT / "sphere.png");
    bulletTexture->configure_texture({
            .texture_wrap_s = engine::GLTextureRepeat::IGNORE,
            .texture_wrap_t = engine::GLTextureRepeat::IGNORE,
            .texture_min_filter = engine::GLFilter::LINEAR,
            .texture_mag_filter = engine::GLFilter::LINEAR,
    });
    outShader = engine::GlobalAssetManager.loadShader(
            RESOURCES_ROOT / "shaders" / "light.vert",
            RESOURCES_ROOT / "shaders" / "light.frag"
            );
    outModel = engine::GlobalAssetManager.loadModel(
            RESOURCES_ROOT / "3dobj" / "sphere_smooth.obj",
            outShader
            );
    outModel->meshes[0]->attachTexture(bulletTexture);
}
void prefabs::throwableBulletPrefab(entt::registry &registry,
                                    std::unique_ptr<btDiscreteDynamicsWorld> &world,
                                    std::shared_ptr<engine::Model> &model,
                                    std::shared_ptr<engine::Shader> &shader,
                                    glm::vec3 position,
                                    glm::vec3 impulse) {
    const auto entity = registry.create();

    float scale = 0.3f;
    glm::vec3 size{1.6f*scale};
    registry.emplace<components::Position>(entity, position, scale);
    const auto &cObject = registry.emplace<components::CollisionObject>(entity, components::collisionObject::cube(
                                                                  world,
                                                                  size,
                                                                  position,
                                                                  glm::mat3{1.f},
                                                                  10.f), world.get());
    cObject.applyImpulse(impulse);
    registry.emplace<components::Renderable>(entity, std::vector<std::shared_ptr<engine::Model>>{model}, shader);
    registry.emplace<components::ShortLivedObject>(entity, 10.f);
}
