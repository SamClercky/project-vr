#ifndef OPENGL_TEST2_WINDOW_H
#define OPENGL_TEST2_WINDOW_H

#include <GLFW/glfw3.h>
#include <functional>
#include <glad/glad.h>
#include <memory>

namespace engine {

class Window {
public:
  Window(int width, int height, std::string &&title);
  ~Window();
  void make_current();
  void set_title(std::string &&title);
  void loop(std::function<void(const uint64_t)> callback);

private:
  GLFWwindow *window;
};

}

#endif // OPENGL_TEST2_WINDOW_H
