#include "ViewportUpdateSystem.h"
#include "components/Camera.h"

void systems::update_viewport_system(entt::registry &registry, engine::Window &window) {
    int width, height;
    if (window.get_viewport(width, height)) {
        auto &cam = registry.ctx().get<components::Camera>();
        cam.update_perspective((float)width, (float)height);
    }
}
