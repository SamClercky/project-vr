#ifndef PROJECT_VR_ASSETMANAGER_H
#define PROJECT_VR_ASSETMANAGER_H

#include "Model.h"
#include "Shader.h"
#include "Texture2D.h"
#include <filesystem>
#include <memory>
#include <unordered_map>

#define RESOURCES_ROOT std::filesystem::path{"resources"}
#define RESOURCES_SRC_ROOT std::filesystem::path{"src"}

namespace engine {
    enum class PrimitiveShape {
        Cube, Quad
    };

    class AssetManager {
    public:
        AssetManager()
            : modelStore(Store<Model>{}),
              meshStore(Store<Mesh>{}),
              shaderStore(Store<Shader>{}),
              textureStore(Store<Texture2D>{}) {}
        AssetManager(AssetManager const &other) = delete;
        AssetManager(AssetManager &&other) = delete;

        std::shared_ptr<Texture2D> loadTexture(const std::filesystem::path &path,
                                               TextureConfig &&config = TextureConfig{});
        std::shared_ptr<Texture2D> loadCubeMap(const std::filesystem::path &front,
                                               const std::filesystem::path &back,
                                               const std::filesystem::path &left,
                                               const std::filesystem::path &right,
                                               const std::filesystem::path &top,
                                               const std::filesystem::path &bottom);
        std::shared_ptr<Model> loadModel(const std::filesystem::path &path, std::shared_ptr<Shader> &shader);
        std::shared_ptr<Shader> loadShader(const std::filesystem::path &vertexPath,
                                           const std::filesystem::path &fragmentPath);
        std::shared_ptr<Shader> loadShader(const std::filesystem::path &vertexPath,
                                           const std::filesystem::path &geometryPath,
                                           const std::filesystem::path &fragmentPath);
        std::shared_ptr<Model> loadPrimitive(const std::filesystem::path path,
                                                           PrimitiveShape shape,
                                                           std::shared_ptr<Shader> &shader);

        void submitMesh(const std::filesystem::path &path, const std::shared_ptr<Mesh>& mesh) {
            meshStore.insert(std::make_pair(path, mesh));
        }
        void submitModel(const std::filesystem::path &path, const std::shared_ptr<Model>& model) {
            modelStore.insert(std::make_pair(path, model));
        }
        void submitTexture(const std::filesystem::path &path, const std::shared_ptr<Texture2D>& texture2D) {
            textureStore.insert(std::make_pair(path, texture2D));
        }
        void submitShader(const std::filesystem::path &path, const std::shared_ptr<Shader>& shader) {
            shaderStore.insert(std::make_pair(path, shader));
        }
    private:
        template<class T>
        using Store = std::unordered_map<std::filesystem::path, std::shared_ptr<T>>;

        Store<Model> modelStore;
        Store<Mesh> meshStore;
        Store<Shader> shaderStore;
        Store<Texture2D> textureStore;
    };

    extern AssetManager GlobalAssetManager;
}// namespace engine

#endif//PROJECT_VR_ASSETMANAGER_H
