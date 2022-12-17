#include "LightSystem.h"
#include "components/Light.h"

namespace systems {

    void lightSystem(engine::Renderer::RenderGuard& renderer, entt::registry& registry) {
        const auto view = registry.view<components::Light>();
        for (const auto& entity : view) {
            const auto &light = view.get<components::Light>(entity);
            //submit
            renderer.submit_light(light.light);
        }
    }

}