#include "AssetManager.h"
#include "stb_image.h"
#include <iostream>

namespace engine {
    AssetManager GlobalAssetManager = AssetManager{};

    std::shared_ptr<Texture2D> AssetManager::loadTexture(std::filesystem::path &&path, TextureConfig &&config) {
        int width, height, nrChannels;
        uint8_t *data = stbi_load(
                reinterpret_cast<const char *>(path.string().c_str()),
                &width, &height, &nrChannels, 0);

        std::shared_ptr<Texture2D> texture;
        if (data) {
            // TODO: Type should be made dynamic so other kinds of textures can
            // be loaded as well
            texture = std::make_shared<Texture2D>(data, width, height, nrChannels, TextureType::Image);
            textureStore.push_back(texture);
        } else {
            std::stringstream ss;
            ss << "Failed to load texture: " << path;
            std::cerr << ss.str() << std::endl;

            stbi_image_free(data);// make sure it is cleaned
            throw std::exception(ss.str().c_str());
        }
        stbi_image_free(data);

        return texture;
    }

    std::shared_ptr<Model> AssetManager::loadModel(std::filesystem::path &&path) {
        return std::shared_ptr<Model>();
    }

    std::shared_ptr<Shader> AssetManager::loadShader(std::filesystem::path &&vertexPath,
                                                     std::filesystem::path &&fragmentPath) {
        auto shader = std::make_shared<Shader>(
                reinterpret_cast<const char *>(vertexPath.string().c_str()),
                reinterpret_cast<const char *>(fragmentPath.string().c_str()));
        shaderStore.push_back(shader);
        return shader;
    }
}// namespace engine