#include "PaintingPrefab.h"
#include "components/Position.h"
#include "components/Renderable.h"
#include "engine/AssetManager.h"
#include "engine/Model.h"
#include <vector>

static std::shared_ptr<engine::Shader> shader;
static std::shared_ptr<engine::Model> model;

void prefabs::paintingPrefab(entt::registry &registry, glm::vec3 position, glm::mat3 orientation) {
    if (!shader)
        shader = engine::GlobalAssetManager.loadShader(
                RESOURCES_ROOT / "shaders" / "painting.vert",
                RESOURCES_ROOT / "shaders" / "painting.frag"
                );
    if (!model) {
        auto texture = engine::GlobalAssetManager.loadTexture(RESOURCES_ROOT / "mc_painting.jpg", true, engine::TextureConfig {
                .texture_wrap_s = engine::GLTextureRepeat::ClampToEdge,
                .texture_wrap_t = engine::GLTextureRepeat::ClampToEdge,
                .texture_wrap_r = engine::GLTextureRepeat::ClampToEdge,
                .texture_min_filter = engine::GLFilter::LINEAR,
                .texture_mag_filter = engine::GLFilter::LINEAR,
        });
        model = engine::GlobalAssetManager.loadPrimitive(RESOURCES_ROOT / "prefabs" / "LightScreenPrefab.cpp",
                                                         engine::PrimitiveShape::Quad, shader);
        model->meshes[0]->attachTexture(texture);
    }

    const auto entity = registry.create();
    registry.emplace<components::Position>(entity, position, orientation);
    registry.emplace<components::Renderable>(
            entity, std::vector<std::shared_ptr<engine::Model>>{model}, shader);
}
