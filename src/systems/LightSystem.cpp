#include "LightSystem.h"
#include "components/Light.h"
#include "components/Camera.h"

namespace systems {

    void lightSystem(engine::Renderer::RenderGuard& renderer, entt::registry& registry) {
        const auto view = registry.view<components::Light>();
        const auto cam = registry.ctx().get<components::Camera>();
        for (const auto& entity : view) {
            const auto &light = view.get<components::Light>(entity);
            //submit
            renderer.submit_light(light.light);
            renderer.submit_viewPos(cam.position);
        }
    }

}