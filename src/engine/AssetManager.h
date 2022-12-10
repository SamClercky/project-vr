#ifndef PROJECT_VR_ASSETMANAGER_H
#define PROJECT_VR_ASSETMANAGER_H

#include "Model.h"
#include "Shader.h"
#include "Texture2D.h"
#include <filesystem>
#include <memory>

#define RESOURCES_ROOT std::filesystem::path{"resources"}

namespace engine {
    class AssetManager {
    public:
        AssetManager()
            : modelStore(std::vector<std::shared_ptr<Model>>{}),
              meshStore(std::vector<std::shared_ptr<Mesh>>{}),
              shaderStore(std::vector<std::shared_ptr<Shader>>{}),
              textureStore(std::vector<std::shared_ptr<Texture2D>>{}) {}
        AssetManager(AssetManager const &other) = delete;
        AssetManager(AssetManager &&other) = delete;

        std::shared_ptr<Texture2D> loadTexture(std::filesystem::path &&path,
                                               TextureConfig &&config = TextureConfig{});
        std::shared_ptr<Model> loadModel(std::filesystem::path &&path, std::shared_ptr<Shader> &shader);
        std::shared_ptr<Shader> loadShader(std::filesystem::path &&vertexPath,
                                           std::filesystem::path &&fragmentPath);

        void submitMesh(const std::shared_ptr<Mesh>& mesh) {
            meshStore.push_back(mesh);
        }
        void submitModel(const std::shared_ptr<Model>& model) {
            modelStore.push_back(model);
        }
        void submitTexture(const std::shared_ptr<Texture2D>& texture2D) {
            textureStore.push_back(texture2D);
        }
        void submitShader(const std::shared_ptr<Shader>& shader) {
            shaderStore.push_back(shader);
        }
    private:
        std::vector<std::shared_ptr<Model>> modelStore;
        std::vector<std::shared_ptr<Mesh>> meshStore;
        std::vector<std::shared_ptr<Shader>> shaderStore;
        std::vector<std::shared_ptr<Texture2D>> textureStore;
    };

    extern AssetManager GlobalAssetManager;
}// namespace engine

#endif//PROJECT_VR_ASSETMANAGER_H
