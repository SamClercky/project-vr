#include "engine/Renderer.h"
#include "engine/Window.h"
#include "scenes/Scene.h"

#include <iostream>

int main() {
    std::cout << "Starting program" << std::endl;

    auto window = engine::Window(800, 600, std::string{"Hello world"});

    engine::Renderer renderer{};
    scenes::Scene scene{window, renderer};
    window.loop([&](uint64_t deltaTime) {
        scene.update(deltaTime);
        {
            auto renderGuard = renderer.startRender();
            scene.render(renderGuard);
        };

        renderer.render();
    });

    return 0;
}
