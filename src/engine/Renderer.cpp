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

    for (const auto &obj: currFrame.objects) {
        auto meshGuard = obj.asset.mesh->bind();
        auto textureGuard = obj.asset.texture->bind(TextureLocation::Texture0);

        //    auto view = glm::mat4(1.0f);
        //    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        //    auto projection =
        //        glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f,
        //        100.0f);

        const auto &shader = obj.asset.shader;
        shader->use();
        shader->setMat4f(std::string{"model"}, obj.modelView);
        shader->setMat4f(std::string{"view"}, currFrame.view);
        shader->setMat4f(std::string{"projection"}, currFrame.perspective);

        meshGuard.draw();
    }
}

std::shared_ptr<Mesh> Renderer::submitToStore(Mesh &&mesh) {
    auto ptr = std::make_shared<Mesh>(std::move(mesh));
    meshStore.push_back(ptr);
    return ptr;
}
std::shared_ptr<Shader> Renderer::submitToStore(Shader &&shader) {
    auto ptr = std::make_shared<Shader>(std::move(shader));
    shaderStore.push_back(ptr);
    return ptr;
}
std::shared_ptr<Texture2D> Renderer::submitToStore(Texture2D &&texture) {
    auto ptr = std::make_shared<Texture2D>(std::move(texture));
    textureStore.push_back(ptr);
    return ptr;
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

void Renderer::RenderGuard::submit_camera(glm::mat4 perspective,
                                          glm::mat4 view) {
    renderBin.view = view;
    renderBin.perspective = perspective;
}
