#include "lightDebugDraw.h"
#include "components/Light.h"
#include "engine/AssetManager.h"
#include "engine/Model.h"

static std::shared_ptr<engine::Shader> shader;
static std::shared_ptr<engine::Model> model;

// draw red balls on the places that lights are placed into the world
void systems::lightDebugDrawSystem(entt::registry &registry, engine::Renderer::RenderGuard &guard) {
    // init code
    if (!shader)
        shader = engine::GlobalAssetManager.loadShader(RESOURCES_ROOT / "shaders" / "v_shader.glsl",
                                                       RESOURCES_ROOT / "shaders" / "light_debug.frag");
    if (!model) {
        model = engine::GlobalAssetManager.loadModel(RESOURCES_ROOT / "3dobj" / "sphere_low_poly.obj", shader);
    }

    const auto &view = registry.view<components::Light>();
    for (const auto &entity: view) {
        auto &lightComponent = view.get<components::Light>(entity);

        glm::mat4 modelView{.2f};
        modelView[3] = glm::vec4{lightComponent.light.position, 1.f};

        guard.submit(engine::RenderAssetRef {
                .mesh = model->meshes[0],
                .shader = shader,
        }, modelView, false);
    }
}
