#include "CubeMapPrefab.h"
#include "components/Position.h"
#include "components/Renderable.h"
#include "engine/AssetManager.h"
#include "engine/Mesh.h"
#include <vector>

namespace prefabs {

    void prefabs::cubeMapPrefab(std::shared_ptr<engine::Model> &asset, std::shared_ptr<engine::Shader> &shader, entt::registry &registry) {
        auto entity = registry.create();
        registry.emplace<components::Position>(entity, glm::vec3{0.f});
        registry.emplace<components::Renderable>(entity,
                                                 std::vector<std::shared_ptr<engine::Model>>{asset}, shader, false);
    }

    void cubeMapPrefabLoader(std::shared_ptr<engine::Model> &outModel, std::shared_ptr<engine::Shader> &outShader) {
        const auto TEXTURE_ROOT = RESOURCES_ROOT / "cubemaps" / "yokohama3";
        auto texture = engine::GlobalAssetManager.loadCubeMap(
                TEXTURE_ROOT / "posz.jpg",
                TEXTURE_ROOT / "negz.jpg",
                TEXTURE_ROOT / "posx.jpg",
                TEXTURE_ROOT / "negx.jpg",
                TEXTURE_ROOT / "posy.jpg",
                TEXTURE_ROOT / "negy.jpg");

        outShader = engine::GlobalAssetManager.loadShader(
                RESOURCES_ROOT / "shaders" / "cubemap.vert",
                RESOURCES_ROOT / "shaders" / "cubemap.frag");

        outModel = engine::GlobalAssetManager.loadPrimitive(RESOURCES_SRC_ROOT / "prefabs" / "CubeMapPrefab.cpp", engine::PrimitiveShape::Cube, outShader);
        outModel->meshes[0]->attachTexture(texture);
    }
}// namespace prefabs