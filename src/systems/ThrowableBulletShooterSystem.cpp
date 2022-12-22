#include "ThrowableBulletShooterSystem.h"

#include "prefabs/ThrowableBulletPrefab.h"
#include "components/CollisionObject.h"
#include "components/Camera.h"
#include "engine/Model.h"
#include "engine/Shader.h"

static std::shared_ptr<engine::Model> model;
static std::shared_ptr<engine::Shader> shader;

void systems::throwableBulletSystem(entt::registry &registry, engine::Window &window, std::unique_ptr<btDiscreteDynamicsWorld> &world) {
    if (window.is_key_pressed(engine::Window::ButtonDirections::Shoot)) {
        const auto &camera = registry.ctx().get<components::Camera>();

        if (!model || !shader)
            prefabs::loadThrowableBulletPrefab(model, shader);
        auto impulse = camera.get_look_direction() * 10.f;
        prefabs::throwableBulletPrefab(registry, world, model, shader, camera.position, impulse);
    }
}
