#ifndef PROJECT_VR_VIEWPORTUPDATESYSTEM_H
#define PROJECT_VR_VIEWPORTUPDATESYSTEM_H

#include <entt/entt.hpp>
#include "engine/Window.h"

namespace systems {
    void update_viewport_system(entt::registry &registry, engine::Window &window);
}

#endif//PROJECT_VR_VIEWPORTUPDATESYSTEM_H
