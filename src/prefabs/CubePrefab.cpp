#include "CubePrefab.h"

#include "engine/Mesh.h"
#include "components/Position.h"
#include "components/Renderable.h"
#include "components/RotateAnimation.h"
#include <utility>

namespace {

auto cubeVertices = std::vector{
    -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.5f,
    0.5f,  -0.5f, 1.0f,  1.0f,  0.5f,  0.5f,  -0.5f, 1.0f,  1.0f,  -0.5f, 0.5f,
    -0.5f, 0.0f,  1.0f,  -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -0.5f, -0.5f, 0.5f,
    0.0f,  0.0f,  0.5f,  -0.5f, 0.5f,  1.0f,  0.0f,  0.5f,  0.5f,  0.5f,  1.0f,
    1.0f,  0.5f,  0.5f,  0.5f,  1.0f,  1.0f,  -0.5f, 0.5f,  0.5f,  0.0f,  1.0f,
    -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  -0.5f, 0.5f,  0.5f,  1.0f,  0.0f,  -0.5f,
    0.5f,  -0.5f, 1.0f,  1.0f,  -0.5f, -0.5f, -0.5f, 0.0f,  1.0f,  -0.5f, -0.5f,
    -0.5f, 0.0f,  1.0f,  -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  -0.5f, 0.5f,  0.5f,
    1.0f,  0.0f,  0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.5f,  0.5f,  -0.5f, 1.0f,
    1.0f,  0.5f,  -0.5f, -0.5f, 0.0f,  1.0f,  0.5f,  -0.5f, -0.5f, 0.0f,  1.0f,
    0.5f,  -0.5f, 0.5f,  0.0f,  0.0f,  0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  -0.5f,
    -0.5f, -0.5f, 0.0f,  1.0f,  0.5f,  -0.5f, -0.5f, 1.0f,  1.0f,  0.5f,  -0.5f,
    0.5f,  1.0f,  0.0f,  0.5f,  -0.5f, 0.5f,  1.0f,  0.0f,  -0.5f, -0.5f, 0.5f,
    0.0f,  0.0f,  -0.5f, -0.5f, -0.5f, 0.0f,  1.0f,  -0.5f, 0.5f,  -0.5f, 0.0f,
    1.0f,  0.5f,  0.5f,  -0.5f, 1.0f,  1.0f,  0.5f,  0.5f,  0.5f,  1.0f,  0.0f,
    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  -0.5f, 0.5f,  0.5f,  0.0f,  0.0f,  -0.5f,
    0.5f,  -0.5f, 0.0f,  1.0f};

}

void prefabs::cubePrefab(engine::RenderAssetRef asset, entt::registry &registry,
                         glm::vec3 position) {
  auto entity = registry.create();
  registry.emplace<components::Position>(entity, position);
  registry.emplace<components::Renderable>(
      entity, std::vector<engine::RenderAssetRef>{std::move(asset)});
  registry.emplace<components::RotateAnimation>(entity, 10.);
}

engine::RenderAssetRef prefabs::cubePrefabLoader(engine::Renderer &renderer) {
  auto cubeTexture = engine::Texture2D{"resources/container.jpg"};
  cubeTexture.configure_texture({
                                    .texture_wrap_s = engine::GLTextureRepeat::Repeat,
                                    .texture_wrap_t = engine::GLTextureRepeat::Repeat,
                                    .texture_min_filter = engine::GLFilter::LINEAR,
                                    .texture_mag_filter = engine::GLFilter::LINEAR,
                                });

  auto mesh = renderer.submitToStore(engine::Mesh{std::move(cubeVertices)});
  auto texture = renderer.submitToStore(std::move(cubeTexture));
  auto shader = renderer.submitToStore(engine::Shader(
      "resources/shaders/v_shader.glsl", "resources/shaders/f_shader.glsl"));

  return engine::RenderAssetRef{
      .mesh = mesh,
      .shader = shader,
      .texture = texture,
  };
}
