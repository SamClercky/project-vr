#include "InputUpdaterSystem.h"
#include "components/Camera.h"
#include "components/DeltaTime.h"
#include "components/Position.h"

void systems::inputUpdaterSystem(entt::registry &registry, engine::Window &window) {
    glm::vec3 delta_pos{0.f};
    if (window.is_key_pressed(engine::Window::ButtonDirections::Up)) {
        delta_pos.z += 1.0;
    }
    if (window.is_key_pressed(engine::Window::ButtonDirections::Down)) {
        delta_pos.z += -1.0;
    }
    if (window.is_key_pressed(engine::Window::ButtonDirections::Right)) {
        delta_pos.x += -1.0;
    }
    if (window.is_key_pressed(engine::Window::ButtonDirections::Left)) {
        delta_pos.x += +1.0;
    }
    glm::vec2 mouse_position;
    window.get_cursor_position(mouse_position);

    auto &cam = registry.ctx().get<components::Camera>();
    auto &dt = registry.ctx().get<components::DeltaTime>();

    auto z = cam.get_look_direction();
    auto x = glm::normalize(glm::cross(cam.get_look_direction(), cam.worldUp));
    auto y = glm::cross(z, x);
    glm::mat3 toCamDirectionTrans{x, y, z};

    auto rotated_delta_pos = toCamDirectionTrans * delta_pos;
    cam.position += rotated_delta_pos * dt.sec();

    float speed{10.f};// degrees per second
    cam.yaw += speed * mouse_position.x * dt.sec();
    cam.pitch -= speed * mouse_position.y * dt.sec();
    cam.pitch = std::clamp(cam.pitch, -80.f, +80.f);// make sure it is not possible to look straight up
}
