#include "Renderer.h"
#include "GLFW/glfw3.h"
#include "engine/AssetManager.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include <glm/glm.hpp>

#include <utility>

using namespace engine;

void renderShadowsMaps(LightObject light, Frame &frame, GLint &depthTexture, glm::mat4 &lightSpaceMatrix);

bool Renderer::isGuardActive = false;
static std::shared_ptr<Shader> depthShader;

Renderer::Renderer() : currFrame(), nextFrame() {}

Renderer::RenderGuard Renderer::startRender() {
    return Renderer::RenderGuard(nextFrame);
}

void renderShadowsMaps(LightObject light, Frame &frame, GLint &depthTexture, glm::mat4 &lightSpaceMatrix) {
    const uint32_t LIGHT_WIDTH = 1024; // Should probably be parametrised in light?
    const uint32_t LIGHT_HEIGHT = 1024;

    uint32_t depthFBO;
    glGenFramebuffers(1, &depthFBO);

    glBindTexture(GL_TEXTURE_2D, depthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, LIGHT_WIDTH, LIGHT_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    float borderFullyLit[] = {1.f, 1.f, 1.f, 1.f};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderFullyLit); // set border to fully lit

    glBindFramebuffer(GL_FRAMEBUFFER, depthFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    // draw
    glViewport(0, 0, LIGHT_WIDTH, LIGHT_HEIGHT);
    glClear(GL_DEPTH_BUFFER_BIT);

    float near_plane = 1.0f, far_plane = 30.f;
    auto perspective = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
//    auto perspective = glm::perspective(glm::radians(90.0f), 1.f, near_plane, far_plane);
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
        if (!obj.hasShadow)
            continue ; // skip all objects without shadow
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

    glCullFace(GL_FRONT);

    // enlarge light containers if needed
    if (currFrame.lights.size() > depthTextures.size()) {
        auto amount = currFrame.lights.size() - depthTextures.size();
        std::vector<GLuint> textures(amount, 0);
        glGenTextures((int)amount, textures.data());
        depthTextures.insert(depthTextures.end(), textures.begin(), textures.end());
    }
    if (currFrame.lights.size() > lightTransforms.size()) {
        auto amount = currFrame.lights.size() - lightTransforms.size();
        std::vector<glm::mat4> temp{amount, glm::mat4{1.f}};
        lightTransforms.insert(lightTransforms.end(), temp.begin(), temp.end());
    }

    for (int i = 0; i < currFrame.lights.size(); i++) {
        auto &light = currFrame.lights[i];
        auto &depthTexture = depthTextures[i];
        auto &lightTransform = lightTransforms[i];
        renderShadowsMaps(light, currFrame, depthTexture, lightTransform);
    }

    glCullFace(GL_BACK);
    glViewport(0, 0, (GLint)viewWidth, (GLint)viewHeight);
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

            shader->setMat4f(std::format("lights[{0}].lightTransform", i), lightTransforms[i]);
        }
        shader->setInt("numLights", (int)currFrame.lights.size());

        // bind textures
        BoundedTexture2DGuard guards[16];
        for (uint32_t i = 0; i < meshGuard.textures.size() && i < 16; i++) {
            guards[i] = meshGuard.textures[i]->bind(i);
        }

        // bind shadows
        for (int i = 0; i < currFrame.lights.size(); i++) {
            const int shadowLocation = (int)meshGuard.textures.size() + i;
            glActiveTexture(GL_TEXTURE0 + shadowLocation);
            glBindTexture(GL_TEXTURE_2D, depthTextures[i]);
            shader->setInt(std::format("lights[{0}].shadowMap", i), shadowLocation);
        }

        meshGuard.draw();
    }
}

Renderer::RenderGuard::RenderGuard(Frame &renderBin) : renderBin(renderBin) {
    isGuardActive = true;
}
Renderer::RenderGuard::~RenderGuard() { isGuardActive = false; }

void Renderer::RenderGuard::submit(RenderAssetRef &&obj, glm::mat4 modelView) { submit(std::move(obj), modelView, true); }
void Renderer::RenderGuard::submit(RenderAssetRef &&obj, glm::mat4 modelView, bool hasShadow) {
    renderBin.objects.push_back({
            .modelView = modelView,
            .asset = std::move(obj),
            .hasShadow = hasShadow,
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
