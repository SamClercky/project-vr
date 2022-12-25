#ifndef OPENGL_TEST2_RENDERER_H
#define OPENGL_TEST2_RENDERER_H

#include "Mesh.h"
#include "Shader.h"
#include "Texture2D.h"

namespace engine {

    struct RenderAssetRef {
        std::shared_ptr<Mesh> mesh;
        std::shared_ptr<Shader> shader;
    };

    struct RenderObject {
        glm::mat4 modelView;
        RenderAssetRef asset;
        bool hasShadow;
    };

    struct LightObject {
        //components::DirLight dirLight;       
        glm::vec3 position;
        glm::vec3 direction;

        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;

        float constant;
        float linear;
        float quadratic;
    };

    struct Frame {
        glm::mat4 view;
        glm::mat4 perspective;
        std::vector<RenderObject> objects;
        std::vector<LightObject> lights;
    };

    class Renderer {
    public:
        struct RenderGuard {
            explicit RenderGuard(Frame &renderBin);
            ~RenderGuard();

            void submit(RenderAssetRef &&obj, glm::mat4 modelView);
            void submit(RenderAssetRef &&obj, glm::mat4 modelView, bool hasShadow);
            void submit_camera(glm::mat4 perspective, glm::mat4 view);
            void submit_light(LightObject li);

        private:
            Frame &renderBin;
        };

        Renderer();
        ~Renderer() = default;

        RenderGuard startRender();

        void render(uint32_t viewWidth, uint32_t viewHeight);

    private:
        static bool isGuardActive;
        Frame currFrame;
        Frame nextFrame;
        std::vector<GLint> depthTextures{};
        std::vector<glm::mat4> lightTransforms{};
    };

}// namespace engine

#endif// OPENGL_TEST2_RENDERER_H
