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
    //
    for (const auto &obj: currFrame.objects) {
        auto meshGuard = obj.asset.mesh->bind();

        const auto &shader = obj.asset.shader;
        shader->use(); 
        shader->setMat4f(std::string{"model"}, obj.modelView);
        shader->setMat4f(std::string{"view"}, currFrame.view);
        shader->setMat4f(std::string{"projection"}, currFrame.perspective);
        
        for (uint32_t i = 0; i < currFrame.lights.size(); i++) {
            const auto &light = currFrame.lights[i];
            shader->setVec3f(std::format("lights.position[{0}]", i), light.position);
            shader->setVec3f(std::format("lights.direction[{0}]", i), light.direction);

            shader->setVec3f(std::format("lights.ambient[{0}]", i), light.ambient);
            shader->setVec3f(std::format("lights.diffuse[{0}]", i), light.diffuse);
            shader->setVec3f(std::format("lights.specular[{0}]", i), light.specular);

            shader->setFloat(std::format("lights.constant[{0}]", i), light.constant);
            shader->setFloat(std::format("lights.linear[{0}]", i), light.linear);
            shader->setFloat(std::format("lights.quadratic[{0}]", i), light.quadratic);
        }
        shader->setInt("numLights", currFrame.lights.size());

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
void Renderer::RenderGuard::submit_light(LightObject li) {
    renderBin.lights.push_back(li);
}

void Renderer::RenderGuard::submit_camera(glm::mat4 perspective,
                                          glm::mat4 view) {
    renderBin.view = view;
    renderBin.perspective = perspective;
}
