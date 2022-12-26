#include "Renderer.h"
#include "GLFW/glfw3.h"
#include "engine/AssetManager.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include <glm/glm.hpp>

#include <iostream>
#include <utility>

using namespace engine;

void renderShadowsMaps(LightObject light, Frame &frame, GLint &depthTexture, glm::mat4 &lightSpaceMatrix);

bool Renderer::isGuardActive = false;
static std::shared_ptr<Shader> depthShader;
static std::shared_ptr<Shader> postProcessingShader;
static std::shared_ptr<Model> screenQuad;

Renderer::Renderer() : currFrame(), nextFrame() {
    postProcessingShader = GlobalAssetManager.loadShader(RESOURCES_ROOT / "shaders" / "post.vert", RESOURCES_ROOT / "shaders" / "post.frag");
    screenQuad = GlobalAssetManager.loadPrimitive(RESOURCES_SRC_ROOT / "engine" / "Renderer.cpp", PrimitiveShape::Quad, postProcessingShader);
}

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
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, LIGHT_WIDTH, LIGHT_HEIGHT);
    glClear(GL_DEPTH_BUFFER_BIT);

    float near_plane = 1.0f, far_plane = 30.f;
    auto perspective = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
    auto view = glm::lookAt(light.position,
                                  light.position + light.direction,
                                  glm::vec3( 0.0f, 1.0f,  0.0f));
    if (glm::length(light.direction) == 0.f) { // quick and dirty way to implement point light
        perspective = glm::perspective(glm::radians(360.0f), 1.f, near_plane, far_plane);
    }
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
    if (viewWidth != prevViewWidth || viewHeight != prevViewHeight) {
        viewportChanged(viewWidth, viewHeight);
    }

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

    // render to framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);
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
        shader->setVec3f("viewPosition", glm::vec3{currFrame.view[3]});

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

            shader->setFloat(std::format("lights[{0}].cutOff", i), light.cutOff);
            shader->setFloat(std::format("lights[{0}].outerCutOff", i), light.outerCutOff);
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
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // render to screen
    glClearColor(1.f, 1.f, 1.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);

    {
        auto guard = screenQuad->meshes[0]->bind();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texColorBuffer);

        postProcessingShader->use();
        postProcessingShader->setFloat("time", static_cast<float>(glfwGetTime()));
        postProcessingShader->setFloat("screenWidth", static_cast<float>(viewWidth));
        postProcessingShader->setFloat("screenHeight", static_cast<float>(viewHeight));
        postProcessingShader->setInt("screenTexture", 0); // bind screen to correct texture

        guard.draw();
    }
}

void Renderer::viewportChanged(uint32_t viewWidth, uint32_t viewHeight) {
    prevViewHeight = viewHeight;
    prevViewWidth = viewWidth;

    // do some cleanup
    if (texColorBuffer != -1)
        glDeleteTextures(1, &texColorBuffer);
    if (rbo != -1)
        glDeleteRenderbuffers(1, &rbo);

    // fbo
    if (fbo == -1) // only first time, generate new fbo
        glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    // texture
    glGenTextures(1, &texColorBuffer);
    glBindTexture(GL_TEXTURE_2D, texColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (GLsizei)viewWidth, (GLsizei)viewHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);

    // rbo
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, (GLsizei)viewWidth, (GLsizei)viewHeight);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cerr << "ERROR::FRAMEBUFFER not complete" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0); // cleanup
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
