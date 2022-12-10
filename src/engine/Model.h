#ifndef PROJECT_VR_MODEL_H
#define PROJECT_VR_MODEL_H

#include <vector>
#include <memory>
#include "Mesh.h"
#include "Renderer.h"

namespace engine {
    class Model {
    public:
        explicit Model(std::vector<std::shared_ptr<Mesh>> &&meshes)
                : meshes(meshes) {}

        void draw(Renderer::RenderGuard &guard,
                  std::shared_ptr<Shader> const &shader,
                  const glm::mat4 position,
                  const std::vector<glm::mat4> &childPositions) const {
            auto minAmount = childPositions.size() < meshes.size() ? childPositions.size() :
                                                                   meshes.size();
            for (uint32_t i = 0; i < minAmount; i++)
                guard.submit({
                                     .mesh = meshes[i],
                                     .shader = shader,
                             }, position * childPositions[i]);

            if (meshes.size() < minAmount) return; // no more meshes -> stop
            for (uint32_t i = minAmount; i < meshes.size(); i++)
                guard.submit({
                        .mesh = meshes[i],
                        .shader = shader,
                }, position);
        }
    private:
        std::vector<std::shared_ptr<Mesh>> meshes;
    };
}

#endif//PROJECT_VR_MODEL_H
