#include "CameraSystem.h"
#include "components/Camera.h"
#include "components/Position.h"

namespace systems {

    void cameraSystem(engine::Renderer::RenderGuard &renderer, entt::registry &registry) {
        auto &cam = registry.ctx().get<components::Camera>();
        renderer.submit_camera(
                cam.perspective,
                glm::lookAt(cam.position, cam.position + cam.get_look_direction(), cam.worldUp));
    }

}// namespace systems