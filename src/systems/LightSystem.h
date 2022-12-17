#ifndef PROJECT_VR_LIGHTSYSTEM_H
#define PROJECT_VR_LIGHTSYSTEM_H

#include "engine/Renderer.h"
#include <entt/entt.hpp>

namespace systems {

	void lightSystem(engine::Renderer::RenderGuard &renderer, entt::registry &registry);

}


#endif// !PROJECT_VR_LIGHTSYSTEM_H
