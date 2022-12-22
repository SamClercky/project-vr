#ifndef OPENGL_TEST2_WINDOW_H
#define OPENGL_TEST2_WINDOW_H

#include <GLFW/glfw3.h>
#include <functional>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <memory>
#include <string>

namespace engine {

    class Window {
    public:
        enum class ButtonDirections {
            Up,
            Down,
            Right,
            Left,
            Shoot,
        };

        Window(int width, int height, std::string &&title);
        ~Window();
        void make_current();
        void set_title(std::string &&title);
        void loop(const std::function<void(const uint64_t)> &callback);

        // true if pressed
        bool is_key_pressed(ButtonDirections key);
        // Get current cursor position `out &out_position` and returned bool is
        // if_changed
        bool get_cursor_position(glm::vec2 &out_position);

        // returns true if it has changed
        bool get_viewport(int &width, int &height);

        [[nodiscard]] static bool is_debug_mode() ;
    private:
        GLFWwindow *m_window;
        glm::vec2 m_mouse_position;
    };

}// namespace engine

#endif// OPENGL_TEST2_WINDOW_H
