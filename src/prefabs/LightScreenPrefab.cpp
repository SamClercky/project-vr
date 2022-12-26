#include "LightScreenPrefab.h"
#include "engine/Model.h"
#include <vector>
#include "engine/AssetManager.h"
#include "components/Position.h"
#include "components/Renderable.h"

static std::shared_ptr<engine::Shader> shader;
static std::shared_ptr<engine::Model> model;

void prefabs::screenPrefab(entt::registry &registry, glm::vec3 position, glm::mat3 orientation) {
    if (!shader)
        shader = engine::GlobalAssetManager.loadShader(
                RESOURCES_ROOT / "shaders" / "painting.vert",
                RESOURCES_ROOT / "shaders" / "painting.frag"
                );
    if (!model)
        model = engine::GlobalAssetManager.loadPrimitive(RESOURCES_ROOT / "prefabs" / "LightScreenPrefab.cpp",
                                                         engine::PrimitiveShape::Quad, shader);

    const auto entity = registry.create();
    registry.emplace<components::Position>(entity, position, orientation);
    registry.emplace<components::Renderable>(
            entity, std::vector<std::shared_ptr<engine::Model>>{model}, shader);
}
