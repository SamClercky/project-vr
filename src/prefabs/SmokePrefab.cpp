#include "SmokePrefab.h"

#include "components/Position.h"
#include "components/Renderable.h"
#include "components/RotateAnimation.h"
#include "engine/Mesh.h"
#include "engine/AssetManager.h"

namespace {

    auto smokeCube = std::vector{
            engine::Vertex{
                    -0.5f,
                    -0.5f,
                    -0.5f,
                    0.0f,
                    0.0f,
            },
            engine::Vertex{
                    0.5f,
                    -0.5f,
                    -0.5f,
                    1.0f,
                    0.0f,
            },
            engine::Vertex{
                    0.5f,
                    0.5f,
                    -0.5f,
                    1.0f,
                    1.0f,
            },
            engine::Vertex{
                    0.5f,
                    0.5f,
                    -0.5f,
                    1.0f,
                    1.0f,
            },
            engine::Vertex{
                    -0.5f,
                    0.5f,
                    -0.5f,
                    0.0f,
                    1.0f,
            },
            engine::Vertex{
                    -0.5f,
                    -0.5f,
                    -0.5f,
                    0.0f,
                    0.0f,
            },
            engine::Vertex{
                    -0.5f,
                    -0.5f,
                    0.5f,
                    0.0f,
                    0.0f,
            },
            engine::Vertex{
                    0.5f,
                    -0.5f,
                    0.5f,
                    1.0f,
                    0.0f,
            },
            engine::Vertex{
                    0.5f,
                    0.5f,
                    0.5f,
                    1.0f,
                    1.0f,
            },
            engine::Vertex{
                    0.5f,
                    0.5f,
                    0.5f,
                    1.0f,
                    1.0f,
            },
            engine::Vertex{
                    -0.5f,
                    0.5f,
                    0.5f,
                    0.0f,
                    1.0f,
            },
            engine::Vertex{
                    -0.5f,
                    -0.5f,
                    0.5f,
                    0.0f,
                    0.0f,
            },
            engine::Vertex{
                    -0.5f,
                    0.5f,
                    0.5f,
                    1.0f,
                    0.0f,
            },
            engine::Vertex{
                    -0.5f,
                    0.5f,
                    -0.5f,
                    1.0f,
                    1.0f,
            },
            engine::Vertex{
                    -0.5f,
                    -0.5f,
                    -0.5f,
                    0.0f,
                    1.0f,
            },
            engine::Vertex{
                    -0.5f,
                    -0.5f,
                    -0.5f,
                    0.0f,
                    1.0f,
            },
            engine::Vertex{
                    -0.5f,
                    -0.5f,
                    0.5f,
                    0.0f,
                    0.0f,
            },
            engine::Vertex{
                    -0.5f,
                    0.5f,
                    0.5f,
                    1.0f,
                    0.0f,
            },
            engine::Vertex{
                    0.5f,
                    0.5f,
                    0.5f,
                    1.0f,
                    0.0f,
            },
            engine::Vertex{
                    0.5f,
                    0.5f,
                    -0.5f,
                    1.0f,
                    1.0f,
            },
            engine::Vertex{
                    0.5f,
                    -0.5f,
                    -0.5f,
                    0.0f,
                    1.0f,
            },
            engine::Vertex{
                    0.5f,
                    -0.5f,
                    -0.5f,
                    0.0f,
                    1.0f,
            },
            engine::Vertex{
                    0.5f,
                    -0.5f,
                    0.5f,
                    0.0f,
                    0.0f,
            },
            engine::Vertex{
                    0.5f,
                    0.5f,
                    0.5f,
                    1.0f,
                    0.0f,
            },
            engine::Vertex{
                    -0.5f,
                    -0.5f,
                    -0.5f,
                    0.0f,
                    1.0f,
            },
            engine::Vertex{
                    0.5f,
                    -0.5f,
                    -0.5f,
                    1.0f,
                    1.0f,
            },
            engine::Vertex{
                    0.5f,
                    -0.5f,
                    0.5f,
                    1.0f,
                    0.0f,
            },
            engine::Vertex{
                    0.5f,
                    -0.5f,
                    0.5f,
                    1.0f,
                    0.0f,
            },
            engine::Vertex{
                    -0.5f,
                    -0.5f,
                    0.5f,
                    0.0f,
                    0.0f,
            },
            engine::Vertex{
                    -0.5f,
                    -0.5f,
                    -0.5f,
                    0.0f,
                    1.0f,
            },
            engine::Vertex{
                    -0.5f,
                    0.5f,
                    -0.5f,
                    0.0f,
                    1.0f,
            },
            engine::Vertex{
                    0.5f,
                    0.5f,
                    -0.5f,
                    1.0f,
                    1.0f,
            },
            engine::Vertex{
                    0.5f,
                    0.5f,
                    0.5f,
                    1.0f,
                    0.0f,
            },
            engine::Vertex{
                    0.5f,
                    0.5f,
                    0.5f,
                    1.0f,
                    0.0f,
            },
            engine::Vertex{
                    -0.5f,
                    0.5f,
                    0.5f,
                    0.0f,
                    0.0f,
            },
            engine::Vertex{-0.5f, 0.5f, -0.5f, 0.0f, 1.0f}};

}

void prefabs::smokePrefab(std::shared_ptr<engine::Model> &asset,
                         std::shared_ptr<engine::Shader> &shader,
                         entt::registry &registry,
                         glm::vec3 position) {
    auto entity = registry.create();
    registry.emplace<components::Position>(entity, position);
    registry.emplace<components::Renderable>(
            entity, std::vector<std::shared_ptr<engine::Model>>{asset}, shader);
    registry.emplace<components::RotateAnimation>(entity, 200.);
}

void prefabs::smokePrefabLoader(std::shared_ptr<engine::Model> &outModel,
                               std::shared_ptr<engine::Shader> &outShader) {
    auto smokeTexture = engine::GlobalAssetManager.loadTexture(RESOURCES_ROOT / "perlin_noise_small.png");
    smokeTexture->configure_texture({
            .texture_wrap_s = engine::GLTextureRepeat::Repeat,
            .texture_wrap_t = engine::GLTextureRepeat::Repeat,
            .texture_min_filter = engine::GLFilter::LINEAR,
            .texture_mag_filter = engine::GLFilter::LINEAR,
    });

    outShader = engine::GlobalAssetManager.loadShader(
            RESOURCES_ROOT / "shaders" / "smoke.vert",
            RESOURCES_ROOT / "shaders" / "smoke.geom",
            RESOURCES_ROOT / "shaders" / "smoke.frag");

    outModel = engine::GlobalAssetManager.loadModel(RESOURCES_ROOT / "3dobj" / "sphere_smooth.obj", outShader);
    outModel->meshes[0]->attachTexture(smokeTexture);
}
