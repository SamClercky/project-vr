#include "LightSystem.h"
#include "components/DirLight.h"

namespace systems {

    void lightSystem(engine::Renderer::RenderGuard& renderer, entt::registry& registry) {
        const auto view = registry.view<components::DirLight>();
        for (const auto& entity : view) {
            const auto &dirLight = view.get<components::DirLight>(entity);
            //submit
            renderer.submit_dirLight(engine::LightObject{dirLight});
        }
    }

}