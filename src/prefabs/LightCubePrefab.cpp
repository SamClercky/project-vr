#include "LightCubePrefab.h"

#include "engine/Mesh.h"
#include "engine/AssetManager.h"
#include "components/Position.h"
#include "components/Renderable.h"
#include "components/RotateAnimation.h"
#include "components/CollisionObject.h"
#include <entt/entt.hpp>

namespace {

	//change to use vertex, no normals yet
    auto cubeVertices = std::vector{
           engine::Vertex{ -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, },
           engine::Vertex{ 0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,  1.0f, 0.0f, },
           engine::Vertex{ 0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,   1.0f, 1.0f, },
           engine::Vertex{ 0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,   1.0f, 1.0f, },
           engine::Vertex{ -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,  0.0f, 1.0f, },
           engine::Vertex{ -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, },

           engine::Vertex{ -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,   0.0f, 0.0f, },
           engine::Vertex{ 0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,    1.0f, 0.0f, },
           engine::Vertex{ 0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,     1.0f, 1.0f, },
           engine::Vertex{ 0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,     1.0f, 1.0f, },
           engine::Vertex{ -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, },
           engine::Vertex{ -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,   0.0f, 0.0f, },

           engine::Vertex{ -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,   0.0f, 0.0f, },
           engine::Vertex{ -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f,  1.0f, 0.0f, },
           engine::Vertex{ -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, },
           engine::Vertex{ -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, },
           engine::Vertex{ -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f,  0.0f, 1.0f, },
           engine::Vertex{ -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,   0.0f, 0.0f, },

           engine::Vertex{ 0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,     0.0f, 0.0f, },
           engine::Vertex{ 0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f,    1.0f, 0.0f, },
           engine::Vertex{ 0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,   1.0f, 1.0f, },
           engine::Vertex{ 0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,   1.0f, 1.0f, },
           engine::Vertex{ 0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,    0.0f, 1.0f, },
           engine::Vertex{ 0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,     0.0f, 0.0f, },

           engine::Vertex{ -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, },
           engine::Vertex{ 0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,  1.0f, 0.0f, },
           engine::Vertex{ 0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,   1.0f, 1.0f, },
           engine::Vertex{ 0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,   1.0f, 1.0f, },
           engine::Vertex{ -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,  0.0f, 1.0f, },
           engine::Vertex{ -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, },

           engine::Vertex{ -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,   0.0f, 0.0f, },
           engine::Vertex{ 0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,    1.0f, 0.0f, },
           engine::Vertex{ 0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,     1.0f, 1.0f, },
           engine::Vertex{ 0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,     1.0f, 1.0f, },
           engine::Vertex{ -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,    0.0f, 1.0f, },
           engine::Vertex{ -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,    0.0f, 0.0f, },
    };

    //define material ig...
}

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
    registry.emplace<components::CollisionObject>(entity, components::collisionobject::cube(world, glm::vec3{1.f}, position, glm::mat3{1.f}, 1.0));
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
    auto mesh = std::make_shared<engine::Mesh>(
            std::move(cubeVertices),
            std::vector<uint32_t>{},
            std::vector<std::shared_ptr<engine::Texture2D>>{cubeTexture},
            outShader);
    engine::GlobalAssetManager.submitMesh(RESOURCES_SRC_ROOT / "prefabs" / "LightCube.cpp", mesh);
    outModel = std::make_shared<engine::Model>(std::vector<std::shared_ptr<engine::Mesh>>{mesh});
}