#ifndef OPENGL_TEST2_RENDERER_H
#define OPENGL_TEST2_RENDERER_H

#include "Mesh.h"
#include "Shader.h"
#include "Texture2D.h"
#include "components/DirLight.h"

namespace engine {

    struct RenderAssetRef {
        std::shared_ptr<Mesh> mesh;
        std::shared_ptr<Shader> shader;
    };

    struct RenderObject {
        glm::mat4 modelView;
        RenderAssetRef asset;
    };

    struct LightObject {
        components::DirLight dirLight;       
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

            void submit(RenderAssetRef obj, glm::mat4 modelView);
            void submit_camera(glm::mat4 perspective, glm::mat4 view);
            void submit_dirLight(LightObject li);

        private:
            Frame &renderBin;
        };

        Renderer();
        ~Renderer() = default;

        RenderGuard startRender();

        void render();

    private:
        static bool isGuardActive;
        Frame currFrame;
        Frame nextFrame;

    };

}// namespace engine

#endif// OPENGL_TEST2_RENDERER_H
