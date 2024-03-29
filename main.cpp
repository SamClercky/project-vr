#include "engine/Renderer.h"
#include "engine/Window.h"
#include "scenes/Scene.h"

#include <iostream>

int main() {
    std::cout << "Starting program" << std::endl;

    auto window = engine::Window(800, 600, std::string{"Project VR"});

    engine::Renderer renderer{};
    scenes::Scene scene{window, renderer};
    window.loop([&](uint64_t deltaTime, uint32_t viewWidth, uint32_t viewHeight) {
        scene.update(deltaTime, viewWidth, viewHeight);
        {
            auto renderGuard = renderer.startRender();
            scene.render(renderGuard);
        };

        renderer.render(viewWidth, viewHeight);
    });

    return 0;
}
