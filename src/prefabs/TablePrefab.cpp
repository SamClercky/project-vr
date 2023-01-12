#include "TablePrefab.h"

#include "engine/Mesh.h"
#include "engine/AssetManager.h"
#include "components/Position.h"
#include "components/Renderable.h"
#include "components/RotateAnimation.h"
#include "components/CollisionObject.h"
#include <entt/entt.hpp>

void prefabs::lightCubePrefab(std::shared_ptr<engine::Model> &asset,
                              std::shared_ptr<engine::Shader> &shader,
                              entt::registry &registry,
                              glm::vec3 position,
                              std::unique_ptr<btDiscreteDynamicsWorld> &world) {
    auto entity = registry.create();
    registry.emplace<components::Position>(entity, position);
    registry.emplace<components::Renderable>(
            entity, std::vector<std::shared_ptr<engine::Model>>{asset}, shader);
    registry.emplace<components::RotateAnimation>(entity, 10.f);
    glm::vec3 size{2.f, 1.2f, 4.8f};
    registry.emplace<components::CollisionObject>(entity, components::collisionObject::cube(world, size, position, glm::mat3{1.f}, 1.0),
                                                  world.get(), glm::vec3{0.05f, .9f, .15f});
}

void prefabs::lightCubePrefabLoader(std::shared_ptr<engine::Model>& outModel,
    std::shared_ptr<engine::Shader>& outShader) {
    auto cubeTexture = engine::GlobalAssetManager.loadTexture(RESOURCES_ROOT / "container.jpg");
    cubeTexture->configure_texture({
            .texture_wrap_s = engine::GLTextureRepeat::Repeat,
            .texture_wrap_t = engine::GLTextureRepeat::Repeat,
            .texture_min_filter = engine::GLFilter::LINEAR,
            .texture_mag_filter = engine::GLFilter::LINEAR,
    });

    outShader = engine::GlobalAssetManager.loadShader(
            RESOURCES_ROOT / "shaders" / "light.vert", RESOURCES_ROOT / "shaders" / "light.frag");
//    outModel = engine::GlobalAssetManager.loadPrimitive(RESOURCES_SRC_ROOT / "prefabs" / "LightCubePrefab.cpp",
//                                                        engine::PrimitiveShape::Cube, outShader);
    outModel = engine::GlobalAssetManager.loadModel(RESOURCES_ROOT / "3dobj" / "table.obj", outShader);
    outModel->meshes[0]->attachTexture(cubeTexture);
}