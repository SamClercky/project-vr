#include "InputUpdaterSystem.h"
#include "BulletCollision/NarrowPhaseCollision/btRaycastCallback.h"
#include "components/Camera.h"
#include "components/CollisionObject.h"
#include "components/DeltaTime.h"
#include "components/FollowedByCamera.h"
#include "components/GameStateGlobals.h"
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
    auto &gameState = registry.ctx().get<components::GameStateGlobals>();

    // cam coords
    auto z = cam.get_look_direction();
    auto x = glm::normalize(glm::cross(cam.get_look_direction(), cam.worldUp));
    auto y = glm::cross(z, x);

    glm::mat3 toCamDirectionTrans{x, y, z};
    auto rotated_delta_pos = toCamDirectionTrans * delta_pos; // intended direction from player

    if (window.is_key_pressed(engine::Window::ButtonDirections::Fly)) {
        gameState.isFreeCam = !gameState.isFreeCam;
    }
    if (gameState.isFreeCam) {
        cam.position += 10.f * rotated_delta_pos * dt.sec();
    } else {
        auto playerView = registry.view<components::FollowedByCamera,
                                        components::Position,
                                        components::CollisionObject>();
        for (const auto &player: playerView) {
            auto &cObject = playerView.get<components::CollisionObject>(player);
            auto &position = playerView.get<components::Position>(player);

            // handle horizontal movement
            if (glm::length(rotated_delta_pos) > 0.f) {
                auto displacement = 10.f * dt.sec() * btVector3{rotated_delta_pos.x, 0.f, rotated_delta_pos.z};

                btVector3 rayFrom = btVector3{cam.position.x, cam.position.y, cam.position.z};
                btVector3 rayTo = rayFrom + displacement;
                btCollisionWorld::ClosestRayResultCallback rayResult{rayFrom, rayTo};
                rayResult.m_flags |= btTriangleRaycastCallback::kF_FilterBackfaces;
                cObject.world->rayTest(rayFrom, rayTo, rayResult);
                if (rayResult.hasHit()) { // make sure that if there is a hit, do not cross it
                    displacement = lerp(btVector3{0.f, 0.f, 0.f}, displacement, rayResult.m_closestHitFraction) - displacement.normalize()*.6f;
                }

                cObject.body->translate(displacement);
            }

            // handle jumping
            btVector3 rayFrom = btVector3{cam.position.x, cam.position.y-1.f, cam.position.z};
            btVector3 rayTo = rayFrom + btVector3{0.f, -1.f, 0.f};
            btCollisionWorld::ClosestRayResultCallback rayResult{rayFrom, rayTo};
            rayResult.m_flags |= btTriangleRaycastCallback::kF_FilterBackfaces;
            cObject.world->rayTest(rayFrom, rayTo, rayResult);
            if (rayResult.hasHit() && window.is_key_pressed(engine::Window::ButtonDirections::Jump))
                cObject.body->applyCentralImpulse(btVector3{0.f, 50.f, 0.f});

            cam.position = position.get_translation();
        }
    }

    float speed{30.f};// degrees per second
    cam.yaw += speed * mouse_position.x * dt.sec();
    cam.pitch -= speed * mouse_position.y * dt.sec();
    cam.pitch = std::clamp(cam.pitch, -80.f, +80.f);// make sure it is not possible to look straight up
}
