#include "CubePrefab.h"

#include "components/Position.h"
#include "components/Renderable.h"
#include "components/RotateAnimation.h"
#include "engine/Mesh.h"
#include <utility>
#include "engine/AssetManager.h"

namespace {

    auto cubeVertices = std::vector{
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

void prefabs::cubePrefab(engine::Model asset,
                         std::shared_ptr<engine::Shader> &shader,
                         entt::registry &registry,
                         glm::vec3 position) {
    auto entity = registry.create();
    registry.emplace<components::Position>(entity, position);
    registry.emplace<components::Renderable>(
            entity, std::vector<engine::Model>{std::move(asset)}, shader);
    registry.emplace<components::RotateAnimation>(entity, 10.);
}

void prefabs::cubePrefabLoader(engine::Renderer &renderer,
                               engine::Model &outModel,
                               std::shared_ptr<engine::Shader> &outShader) {
    auto cubeTexture = engine::GlobalAssetManager.loadTexture(RESOURCES_ROOT / "container.jpg");
    cubeTexture->configure_texture({
            .texture_wrap_s = engine::GLTextureRepeat::Repeat,
            .texture_wrap_t = engine::GLTextureRepeat::Repeat,
            .texture_min_filter = engine::GLFilter::LINEAR,
            .texture_mag_filter = engine::GLFilter::LINEAR,
    });

    outShader = engine::GlobalAssetManager.loadShader(
            RESOURCES_ROOT / "shaders" / "v_shader.glsl", RESOURCES_ROOT / "shaders" / "f_shader.glsl");
    auto mesh = std::make_shared<engine::Mesh>(
            std::move(cubeVertices),
            std::vector<uint32_t>{},
            std::vector<std::shared_ptr<engine::Texture2D>>{cubeTexture},
            outShader);
    engine::GlobalAssetManager.submitMesh(mesh);

    outModel = engine::Model{std::vector<std::shared_ptr<engine::Mesh>>{mesh}};
}
