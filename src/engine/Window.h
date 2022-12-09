#ifndef OPENGL_TEST2_WINDOW_H
#define OPENGL_TEST2_WINDOW_H

#include <GLFW/glfw3.h>
#include <functional>
#include <glad/glad.h>
#include <memory>
#include <glm/glm.hpp>
#include <string>

namespace engine {

class Window {
public:

  enum class ButtonDirections {
    Up, Down, Right, Left
  };

  Window(int width, int height, std::string &&title);
  ~Window();
  void make_current();
  void set_title(std::string &&title);
  void loop(const std::function<void(const uint64_t)>& callback);

  // true if pressed
  bool is_key_pressed(ButtonDirections key);
  // Get current cursor position `out &out_position` and returned bool is
  // if_changed
  bool get_cursor_position(glm::vec2 &out_position);

private:
  GLFWwindow *m_window;
  glm::vec2 m_mouse_position;
};

}

#endif // OPENGL_TEST2_WINDOW_H
