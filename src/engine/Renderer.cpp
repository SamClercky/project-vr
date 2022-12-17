#include "Renderer.h"

#include <utility>

using namespace engine;

bool Renderer::isGuardActive = false;

Renderer::Renderer() : currFrame(), nextFrame() {}

Renderer::RenderGuard Renderer::startRender() {
    return Renderer::RenderGuard(nextFrame);
}

void Renderer::render() {
    if (!isGuardActive) {
        std::swap(currFrame.objects, nextFrame.objects);
        nextFrame.objects.clear();
        currFrame.perspective = nextFrame.perspective;
        currFrame.view = nextFrame.view;
    }

    // current frame only contains new data if nextFrame was ready, otherwise
    // rerender previous frame

    glClearColor(.2f, .3f, .3f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glDepthFunc(GL_LEQUAL);
    //yeet light data in shader..
    for (const auto &obj: currFrame.objects) {
        auto meshGuard = obj.asset.mesh->bind();

        const auto &shader = obj.asset.shader;
        shader->use();
        shader->setMat4f(std::string{"model"}, obj.modelView);
        shader->setMat4f(std::string{"view"}, currFrame.view);
        shader->setMat4f(std::string{"projection"}, currFrame.perspective);

        meshGuard.draw();
    }
}

Renderer::RenderGuard::RenderGuard(Frame &renderBin) : renderBin(renderBin) {
    isGuardActive = true;
}
Renderer::RenderGuard::~RenderGuard() { isGuardActive = false; }

void Renderer::RenderGuard::submit(RenderAssetRef obj, glm::mat4 modelView) {
    renderBin.objects.push_back({
            .modelView = modelView,
            .asset = std::move(obj),
    });
}

//submit light, Dir/Point/.. check subtype?
void Renderer::RenderGuard::submit_dirLight(LightObject li) {
    renderBin.lights.push_back(li);
}

void Renderer::RenderGuard::submit_camera(glm::mat4 perspective,
                                          glm::mat4 view) {
    renderBin.view = view;
    renderBin.perspective = perspective;
}
