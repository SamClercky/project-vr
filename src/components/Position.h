#ifndef OPENGL_TEST2_POSITION_H
#define OPENGL_TEST2_POSITION_H

#include "glm/gtc/matrix_transform.hpp"
#include <glm/glm.hpp>

namespace components {

    struct Position {
        glm::mat4 pos{1.0};

        explicit Position(glm::mat4 M) : pos(M) {}
        Position(glm::vec3 translation, float scale)
            : Position(translation, glm::mat3{1.f}, scale) {}
        explicit Position(glm::vec3 translation)
            : Position(translation, glm::mat3{1.0}) {}
        explicit Position(glm::mat3 rotation) : Position(glm::vec3{0.0}, rotation) {}
        Position(glm::vec3 translation, glm::mat3 rotation): Position(translation, rotation, 1.f) {}
        Position(glm::vec3 translation, glm::mat3 rotation, float scale) {
            pos = glm::mat4{scale * rotation};
            pos = glm::translate(pos, translation);
        }

        void set_scale(float s) {
            auto rot = s*glm::mat3{pos};
            pos = glm::mat4{
                    glm::vec4{rot[0], pos[0][3]},
                    glm::vec4{rot[1], pos[1][3]},
                    glm::vec4{rot[2], pos[2][3]},
                    pos[3]
            };
        }

        [[nodiscard]] glm::mat3 get_rotation() const {
            return glm::mat3{
                    glm::vec3{pos[0]},
                    glm::vec3{pos[1]},
                    glm::vec3{pos[2]},
            };
        }

        [[nodiscard]] glm::vec3 get_translation() const {
            return glm::vec3{pos[3]};
        }

        void set_rotation(glm::mat3 rot) {
            auto prev_rot = glm::mat3{pos};
            auto rot_len = glm::vec3{
                    glm::length(prev_rot[0]),
                    glm::length(prev_rot[1]),
                    glm::length(prev_rot[2]),
            };

            pos = glm::mat4{
                    glm::vec4{rot[0]*rot_len.x, pos[0][3]},
                    glm::vec4{rot[1]*rot_len.y, pos[1][3]},
                    glm::vec4{rot[2]*rot_len.z, pos[2][3]},
                    pos[3]
            };
        }

        void set_translation(glm::vec3 translation) {
            pos[3] = glm::vec4{translation, 1.f};
        }
    };

}// namespace components

#endif// OPENGL_TEST2_POSITION_H
