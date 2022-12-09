#ifndef OPENGL_TEST2_RENDERER_H
#define OPENGL_TEST2_RENDERER_H

#include "Mesh.h"
#include "Shader.h"
#include "Texture2D.h"

namespace engine {

    struct RenderAssetRef {
        std::shared_ptr<Mesh> mesh;
        std::shared_ptr<Shader> shader;
        std::shared_ptr<Texture2D> texture;
    };

    struct RenderObject {
        glm::mat4 modelView;
        RenderAssetRef asset;
    };

    struct Frame {
        glm::mat4 view;
        glm::mat4 perspective;
        std::vector<RenderObject> objects;
    };

    class Renderer {
    public:
        struct RenderGuard {
            explicit RenderGuard(Frame &renderBin);
            ~RenderGuard();

            void submit(RenderAssetRef obj, glm::mat4 modelView);
            void submit_camera(glm::mat4 perspective, glm::mat4 view);

        private:
            Frame &renderBin;
        };

        Renderer();
        ~Renderer() = default;

        RenderGuard startRender();

        void render();

        std::shared_ptr<Mesh> submitToStore(Mesh &&mesh);
        std::shared_ptr<Shader> submitToStore(Shader &&shader);
        std::shared_ptr<Texture2D> submitToStore(Texture2D &&texture2D);

    private:
        static bool isGuardActive;
        Frame currFrame;
        Frame nextFrame;

        std::vector<std::shared_ptr<Mesh>> meshStore;
        std::vector<std::shared_ptr<Shader>> shaderStore;
        std::vector<std::shared_ptr<Texture2D>> textureStore;
    };

}// namespace engine

#endif// OPENGL_TEST2_RENDERER_H
