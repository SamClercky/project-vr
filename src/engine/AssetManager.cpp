#include "AssetManager.h"
#include "stb_image.h"
#include <iostream>
#include <stack>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

namespace engine {

    //change to use vertex, no normals yet
    static auto cubeVertices = std::vector{
            Vertex{ -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f },
            Vertex{  0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f },
            Vertex{  0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f },
            Vertex{  0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f },
            Vertex{ -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f },
            Vertex{ -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f },

            Vertex{ -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f },
            Vertex{  0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f },
            Vertex{  0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f },
            Vertex{  0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f },
            Vertex{ -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f },
            Vertex{ -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f },

            Vertex{ -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f },
            Vertex{ -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f },
            Vertex{ -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f },
            Vertex{ -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f },
            Vertex{ -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f },
            Vertex{ -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f },

            Vertex{  0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f },
            Vertex{  0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f },
            Vertex{  0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f },
            Vertex{  0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f },
            Vertex{  0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f },
            Vertex{  0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f },

            Vertex{ -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f },
            Vertex{  0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f },
            Vertex{  0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f },
            Vertex{  0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f },
            Vertex{ -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f },
            Vertex{ -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f },

            Vertex{ -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f },
            Vertex{  0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f },
            Vertex{  0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f },
            Vertex{  0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f },
            Vertex{ -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f },
            Vertex{ -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f },
    };

    static auto verticesQuad = std::vector<engine::Vertex>{
            engine::Vertex{-1.0f,  1.0f, 0.f,  0.0f, 1.0f},
            engine::Vertex{-1.0f, -1.0f, 0.f,  0.0f, 0.0f},
            engine::Vertex{1.0f, -1.0f, 0.f, 1.0f, 0.0f},

            engine::Vertex{-1.0f,  1.0f, 0.f, 0.0f, 1.0f},
            engine::Vertex{1.0f, -1.0f, 0.f, 1.0f, 0.0f},
            engine::Vertex{1.0f,  1.0f, 0.f, 1.0f, 1.0f}
    };


    AssetManager GlobalAssetManager = AssetManager{};

    std::shared_ptr<Texture2D> AssetManager::loadTexture(const std::filesystem::path &path, TextureConfig &&config) {
        if (textureStore.contains(path))
            return textureStore.at(path);

        int width, height, nrChannels;
        uint8_t *data = stbi_load(
                reinterpret_cast<const char *>(path.string().c_str()),
                &width, &height, &nrChannels, 0);

        std::shared_ptr<Texture2D> texture;
        if (data) {
            // TODO: Type should be made dynamic so other kinds of textures can
            // be loaded as well
            texture = std::make_shared<Texture2D>(data, width, height, nrChannels, TextureType::Image);
            texture->configure_texture(config);
            textureStore.insert(std::make_pair(path, texture));
        } else {
            std::stringstream ss;
            ss << "Failed to load texture: " << path
               << " with reason: " << stbi_failure_reason();
            std::cerr << ss.str() << std::endl;

            stbi_image_free(data);// make sure it is cleaned
            throw std::exception(ss.str().c_str());
        }
        stbi_image_free(data);

        return texture;
    }


    std::shared_ptr<Texture2D> AssetManager::loadCubeMap(const std::filesystem::path &front,
                                                         const std::filesystem::path &back,
                                                         const std::filesystem::path &left,
                                                         const std::filesystem::path &right,
                                                         const std::filesystem::path &top,
                                                         const std::filesystem::path &bottom) {

        const auto key = front / back / left / right / top / bottom;
        if (textureStore.contains(key))
            return textureStore.at(key);

        TextureCubeMap cubeMap{};
        std::filesystem::path paths[] = {front, back, left, right, top, bottom};
        TextureCubeMapSide *sides[] = {&cubeMap.front, &cubeMap.back, &cubeMap.left, &cubeMap.right, &cubeMap.top, &cubeMap.bottom};

        for (uint8_t i = 0; i < 6; i++) {
            auto path = paths[i];
            auto side = sides[i];

            int width, height, nrChannels;
            uint8_t *data = stbi_load(
                    reinterpret_cast<const char *>(path.string().c_str()), &width, &height, &nrChannels, 0);

            if (data) {
                // be loaded as well
                side->data = data;
                side->width = width;
                side->height = height;
            } else {
                std::stringstream ss;
                ss << "Failed to load texture: " << path
                   << " with reason: " << stbi_failure_reason();
                std::cerr << ss.str() << std::endl;

                for (uint8_t j = 0; j <= i; j++)// clear all already loaded textures
                    stbi_image_free(sides[j]->data);
                throw std::exception(ss.str().c_str());
            }
        }

        auto texture = std::make_shared<Texture2D>(cubeMap);
        texture->configure_texture({
                .texture_wrap_s = GLTextureRepeat::ClampToEdge,
                .texture_wrap_t = GLTextureRepeat::ClampToEdge,
                .texture_wrap_r = GLTextureRepeat::ClampToEdge,
                .texture_min_filter = GLFilter::LINEAR,
                .texture_mag_filter = GLFilter::LINEAR,
        });
        textureStore.insert(std::make_pair(key, texture));
        for (auto &side: sides)// clear all textures
            stbi_image_free(side->data);

        return texture;
    }

    std::shared_ptr<Model> AssetManager::loadModel(const std::filesystem::path &path, std::shared_ptr<Shader> &shader) {
        if (modelStore.contains(path))
            return modelStore.at(path);

        Assimp::Importer importer;
        const auto *scene = importer.ReadFile(path.string().c_str(), aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            std::stringstream ss;
            ss << "ERROR::ASSIMP::SCENE_NOT_LOADED::" << importer.GetErrorString();
            std::cerr << ss.str() << std::endl;
            throw std::runtime_error(ss.str());
        }

        // processing nodes -> assimp uses as tree that needs to be processed
        std::stack<aiNode *> recursiveAgenda;
        recursiveAgenda.push(scene->mRootNode);// initial node
        auto model = std::make_shared<Model>(std::vector<std::shared_ptr<Mesh>>{});

        while (!recursiveAgenda.empty()) {
            auto node = recursiveAgenda.top();// get element
            recursiveAgenda.pop();            // remove element

            // process mesh
            for (uint32_t i = 0; i < node->mNumMeshes; i++) {
                auto aMesh = scene->mMeshes[node->mMeshes[i]];

                std::vector<Vertex> vertices;
                std::vector<uint32_t> indices;
                std::vector<std::shared_ptr<Texture2D>> textures;

                // Process vertices
                for (uint32_t j = 0; j < aMesh->mNumVertices; j++) {
                    Vertex vertex{
                            glm::vec3{
                                    aMesh->mVertices[j].x,
                                    aMesh->mVertices[j].y,
                                    aMesh->mVertices[j].z,
                            },
                            glm::vec3{
                                    aMesh->mNormals[j].x,
                                    aMesh->mNormals[j].y,
                                    aMesh->mNormals[j].z,
                            },
                            glm::vec2{
                                    aMesh->mTextureCoords[0][j].x,
                                    aMesh->mTextureCoords[0][j].y,
                            }};
                    vertices.push_back(vertex);
                }

                // Process indices
                for (uint32_t j = 0; j < aMesh->mNumFaces; j++) {
                    auto aFace = aMesh->mFaces[j];
                    for (uint32_t k = 0; k < aFace.mNumIndices; k++) {
                        indices.push_back(aFace.mIndices[k]);
                    }
                }

                // TODO: Process materials (diffuse maps and specular maps)

                // Put everything into the model
                auto mesh = std::make_shared<Mesh>(std::move(vertices), std::move(indices), std::move(textures), shader);
                meshStore.insert(std::make_pair(path, mesh));
                model->meshes.push_back(mesh);
            }

            // recurse to other children
            for (uint32_t i = 0; i < node->mNumChildren; i++) {
                recursiveAgenda.push(node->mChildren[i]);
            }
        }

        importer.FreeScene();

        return model;
    }

    std::shared_ptr<Shader> AssetManager::loadShader(const std::filesystem::path &vertexPath,
                                                     const std::filesystem::path &fragmentPath) {
        const auto key = vertexPath / fragmentPath;
        if (shaderStore.contains(key))
            return shaderStore.at(key);

        auto shader = std::make_shared<Shader>(vertexPath, fragmentPath);
        shaderStore.insert(std::make_pair(key, shader));
        return shader;
    }
    std::shared_ptr<Shader> AssetManager::loadShader(const std::filesystem::path &vertexPath, const std::filesystem::path &geometryPath, const std::filesystem::path &fragmentPath) {
        const auto key = vertexPath / geometryPath / fragmentPath;
        if (shaderStore.contains(key))
            return shaderStore.at(key);

        auto shader = std::make_shared<Shader>(
                vertexPath,
                geometryPath,
                fragmentPath);
        shaderStore.insert(std::make_pair(key, shader));
        return shader;
    }

    std::shared_ptr<Model> AssetManager::loadPrimitive(const std::filesystem::path path,
                                                       PrimitiveShape shape,
                                                       std::shared_ptr<Shader> &shader) {
        std::vector<Vertex> data;
        switch (shape) {
            case PrimitiveShape::Cube: data = cubeVertices; break;
            case PrimitiveShape::Quad: data = verticesQuad; break;
        }
        if (modelStore.contains(path))
            return modelStore.at(path);

        auto mesh = std::make_shared<Mesh>(std::move(data), shader);
        submitMesh(path, mesh);
        auto model = std::make_shared<engine::Model>(std::vector<std::shared_ptr<engine::Mesh>>{mesh});
        submitModel(path, model);

        return model;
    }
}// namespace engine