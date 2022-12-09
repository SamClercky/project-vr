#ifndef OPENGL_TEST2_SCENE_H
#define OPENGL_TEST2_SCENE_H

#include "GLFW/glfw3.h"
#include "engine/Renderer.h"
#include "engine/Window.h"
#include <entt/entt.hpp>

namespace scenes {

    class Scene {
    public:
        explicit Scene(engine::Window &window, engine::Renderer &renderer);
        ~Scene() = default;

        void update(uint64_t deltaTime);
        void render(engine::Renderer::RenderGuard &renderer);

    private:
        entt::registry m_registry;
        engine::Window &m_window_ref;
    };

}// namespace scenes

#endif// OPENGL_TEST2_SCENE_H
