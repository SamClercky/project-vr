#ifndef PROJECT_VR_INPUTUPDATERSYSTEM_H
#define PROJECT_VR_INPUTUPDATERSYSTEM_H

#include "engine/Window.h"
#include <entt/entt.hpp>

namespace systems {
  void inputUpdaterSystem(entt::registry &registry, engine::Window &window);
}

#endif // PROJECT_VR_INPUTUPDATERSYSTEM_H
