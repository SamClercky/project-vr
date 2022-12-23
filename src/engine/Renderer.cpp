#include "Renderer.h"
#include "GLFW/glfw3.h"
#include "engine/AssetManager.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include <glm/glm.hpp>

#include <utility>

using namespace engine;

void renderShadowsMaps(LightObject light, Frame &frame);

bool Renderer::isGuardActive = false;
static std::shared_ptr<Shader> depthShader;

Renderer::Renderer() : currFrame(), nextFrame() {}

Renderer::RenderGuard Renderer::startRender() {
    return Renderer::RenderGuard(nextFrame);
}

void renderShadowsMaps(LightObject light, Frame &frame, uint32_t &depthTexture, glm::mat4 &lightSpaceMatrix) {
    const uint32_t LIGHT_WIDTH = 1024; // Should probably be parametrised in light?
    const uint32_t LIGHT_HEIGHT = 1024;

    uint32_t depthFBO;
    glGenFramebuffers(1, &depthFBO);

    glGenTextures(1, &depthTexture);
    glBindTexture(GL_TEXTURE_2D, depthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, LIGHT_WIDTH, LIGHT_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindFramebuffer(GL_FRAMEBUFFER, depthFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    // draw
    glViewport(0, 0, LIGHT_WIDTH, LIGHT_HEIGHT);
    glClear(GL_DEPTH_BUFFER_BIT);

    float near_plane = 1.0f, far_plane = 7.5f;
//    auto perspective = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
    auto perspective = glm::perspective(glm::radians(45.0f), 1.f, near_plane, far_plane);
    auto view = glm::lookAt(light.position,
                                  light.position + light.direction,
                                  glm::vec3( 0.0f, 1.0f,  0.0f));
    lightSpaceMatrix = perspective * view;

    if (!depthShader)
        depthShader = GlobalAssetManager.loadShader(
                RESOURCES_ROOT / "shaders" / "depth.vert",
                RESOURCES_ROOT / "shaders" / "depth.frag");
    depthShader->use();
    depthShader->setMat4f("view", view);
    depthShader->setMat4f("projection", perspective);

    for (const auto &obj: frame.objects) {
        auto meshGuard = obj.asset.mesh->bind();
        depthShader->setMat4f("model", obj.modelView);
        meshGuard.draw();
    }

    // cleanup
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDeleteFramebuffers(1, &depthFBO);
}

void Renderer::render(uint32_t viewWidth, uint32_t viewHeight) {
    if (!isGuardActive) {
        std::swap(currFrame.objects, nextFrame.objects);
        nextFrame.objects.clear();
        currFrame.perspective = nextFrame.perspective;
        currFrame.view = nextFrame.view;
        std::swap(currFrame.lights, nextFrame.lights);
        nextFrame.lights.clear();
    }

    // current frame only contains new data if nextFrame was ready, otherwise
    // rerender previous frame

    uint32_t shadowMap;
    glm::mat4 lightSpaceMatrix;
    glCullFace(GL_FRONT);
    renderShadowsMaps(currFrame.lights[0], currFrame, shadowMap, lightSpaceMatrix);

    glCullFace(GL_BACK);
    glViewport(0, 0, viewWidth, viewHeight);
    glClearColor(.2f, .3f, .3f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glDepthFunc(GL_LEQUAL); // needed for the cubemap

    for (const auto &obj: currFrame.objects) {
        auto meshGuard = obj.asset.mesh->bind();

        const auto &shader = obj.asset.shader;
        shader->use();
        shader->setMat4f("model", obj.modelView);
        shader->setMat4f("view", currFrame.view);
        shader->setMat4f("projection", currFrame.perspective);
        shader->setFloat("time", static_cast<float>(glfwGetTime()));
        shader->setMat4f("lightSpaceMatrix", lightSpaceMatrix);

        for (uint32_t i = 0; i < currFrame.lights.size(); i++) {
            const auto &light = currFrame.lights[i];
            shader->setVec3f(std::format("lights[{0}].position", i), light.position);
            shader->setVec3f(std::format("lights[{0}].direction", i), light.direction);

            shader->setVec3f(std::format("lights[{0}].ambient", i), light.ambient);
            shader->setVec3f(std::format("lights[{0}].diffuse", i), light.diffuse);
            shader->setVec3f(std::format("lights[{0}].specular", i), light.specular);

            shader->setFloat(std::format("lights[{0}].constant", i), light.constant);
            shader->setFloat(std::format("lights[{0}].linear", i), light.linear);
            shader->setFloat(std::format("lights[{0}].quadratic", i), light.quadratic);
        }
        shader->setInt("numLights", currFrame.lights.size());

        glActiveTexture(GL_TEXTURE0 + meshGuard.textures.size());
        glBindTexture(GL_TEXTURE_2D, shadowMap);

        meshGuard.draw();
    }

    glDeleteTextures(1, &shadowMap);
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
