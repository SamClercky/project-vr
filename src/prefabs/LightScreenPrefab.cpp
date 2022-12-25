#include "LightScreenPrefab.h"
#include "engine/Model.h"
#include <vector>
#include "engine/AssetManager.h"
#include "components/Position.h"
#include "components/Renderable.h"

static auto vertices = std::vector<engine::Vertex>{
                      // positions        // texture coords
        engine::Vertex{0.5f,  0.5f, 0.0f, 1.0f, 1.0f},   // top right
        engine::Vertex{0.5f, -0.5f, 0.0f, 1.0f, 0.0f},   // bottom right
        engine::Vertex{-0.5f, -0.5f, 0.0f, 0.0f, 0.0f},   // bottom left
        engine::Vertex{-0.5f,  0.5f, 0.0f, 0.0f, 1.0f},    // top left
        engine::Vertex{0.5f,  0.5f, 0.0f, 1.0f, 1.0f},   // top right
        engine::Vertex{-0.5f, -0.5f, 0.0f, 0.0f, 0.0f},   // bottom left
};

static std::shared_ptr<engine::Shader> shader;
static std::shared_ptr<engine::Mesh> mesh;

void prefabs::screenPrefab(entt::registry &registry, glm::vec3 position, glm::mat3 orientation) {
    if (!shader)
        shader = engine::GlobalAssetManager.loadShader(
                RESOURCES_ROOT / "shaders" / "painting.vert",
                RESOURCES_ROOT / "shaders" / "painting.frag"
                );
    if (!mesh) {
        mesh = std::make_shared<engine::Mesh>(std::move(vertices), shader);
        engine::GlobalAssetManager.submitMesh(
                RESOURCES_SRC_ROOT / "prefabs" / "LightScreenPrefab.cpp",
                mesh);
    }

    const auto entity = registry.create();
    registry.emplace<components::Position>(entity, position, orientation);
    auto model = std::make_shared<engine::Model>(std::vector<std::shared_ptr<engine::Mesh>>{mesh});
    registry.emplace<components::Renderable>(
            entity, std::vector<std::shared_ptr<engine::Model>>{model}, shader);
}
