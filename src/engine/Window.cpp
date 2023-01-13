#include <iostream>

#include "Window.h"

using namespace engine;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
float get_joystick_value(int glfw_joystick, unsigned int axis);
bool get_joystick_button(int glfw_joystick, unsigned int btn);

static int m_width = 0.f;
static int m_height = 0.f;
static bool hasViewportChanged = true;

static bool was_key_fly_pressed = false;
static bool was_key_shoot_pressed = false;
static bool was_key_debug_pressed = false;
static bool was_key_fullscreen_pressed = false;

static bool key_debug_status = false;
static bool key_fullscreen_status = false;

// Debug setup code from exercises
#ifndef NDEBUG
void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id,
                            GLenum severity, GLsizei length,
                            const char *message, const void *userParam) {
    // ignore non-significant error/warning codes
    if (id == 131169 || id == 131185 || id == 131218 || id == 131204)
        return;

    std::cout << "---------------" << std::endl;
    std::cout << "Debug message (" << id << "): " << message << std::endl;

    switch (source) {
        case GL_DEBUG_SOURCE_API:
            std::cout << "Source: API";
            break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
            std::cout << "Source: Window System";
            break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER:
            std::cout << "Source: Shader Compiler";
            break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:
            std::cout << "Source: Third Party";
            break;
        case GL_DEBUG_SOURCE_APPLICATION:
            std::cout << "Source: Application";
            break;
        case GL_DEBUG_SOURCE_OTHER:
            std::cout << "Source: Other";
            break;
    }
    std::cout << std::endl;

    switch (type) {
        case GL_DEBUG_TYPE_ERROR:
            std::cout << "Type: Error";
            break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            std::cout << "Type: Deprecated Behaviour";
            break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            std::cout << "Type: Undefined Behaviour";
            break;
        case GL_DEBUG_TYPE_PORTABILITY:
            std::cout << "Type: Portability";
            break;
        case GL_DEBUG_TYPE_PERFORMANCE:
            std::cout << "Type: Performance";
            break;
        case GL_DEBUG_TYPE_MARKER:
            std::cout << "Type: Marker";
            break;
        case GL_DEBUG_TYPE_PUSH_GROUP:
            std::cout << "Type: Push Group";
            break;
        case GL_DEBUG_TYPE_POP_GROUP:
            std::cout << "Type: Pop Group";
            break;
        case GL_DEBUG_TYPE_OTHER:
            std::cout << "Type: Other";
            break;
    }
    std::cout << std::endl;

    switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH:
            std::cout << "Severity: high";
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            std::cout << "Severity: medium";
            break;
        case GL_DEBUG_SEVERITY_LOW:
            std::cout << "Severity: low";
            break;
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            std::cout << "Severity: notification";
            break;
    }
    std::cout << std::endl;
    std::cout << std::endl;
}
#endif

Window::Window(int width, int height, std::string &&title) {
    m_width = width;
    m_height = height;
    hasViewportChanged = true;
    // Create the OpenGL context
    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialise GLFW \n");
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifndef NDEBUG
    // create a debug context to help with Debugging
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif

    // Create the window
    m_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (m_window == nullptr) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window\n");
    }

    make_current();

    // load openGL function
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        throw std::runtime_error("Failed to initialize GLAD");
    }

#ifndef NDEBUG
    int flags;
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(glDebugOutput, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr,
                              GL_TRUE);
    }
#endif

    // update viewport size
    glViewport(0, 0, width, height);
    glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);

    // enable Z-buffer
    glEnable(GL_DEPTH_TEST);

    // enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // default blending mode

    // enable multisampling
    glEnable(GL_MULTISAMPLE);

    // set input settings
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Window::make_current() {
    if (m_window != nullptr) {
        glfwMakeContextCurrent(m_window);
    } else {
        throw std::runtime_error("Window not constructed (nullptr)");
    }
}

Window::~Window() {
    glfwDestroyWindow(m_window);
    m_window = nullptr;// make sure no dangling pointers
    glfwTerminate();
}

void Window::set_title(std::string &&title) {
    glfwSetWindowTitle(m_window, title.c_str());
}
void Window::loop(const std::function<void(const uint64_t, const uint32_t, const uint32_t)> &callback) {
    auto prevTime = (uint64_t) (glfwGetTime() * 1000.0);

    // vsync
    glfwSwapInterval(1);
    while (!glfwWindowShouldClose(m_window)) {
        // update fullscreen status if needed
        bool fs_status = glfwGetKey(m_window, GLFW_KEY_F11);
        bool condition = fs_status ^ was_key_fullscreen_pressed && fs_status;
        was_key_fullscreen_pressed = fs_status;

        if (condition) {
            auto *monitor = glfwGetWindowMonitor(m_window);
            if (!monitor) {
                int count;
                auto **monitors = glfwGetMonitors(&count);
                if (count > 0) {
                    monitor = monitors[0];
                    const auto *mode = glfwGetVideoMode(monitor);
                    m_width = mode->width;
                    m_height = mode->height;
                }
            } else {
                monitor = nullptr;
                m_width = 800;
                m_height = 600;
            }
            glfwSetWindowMonitor(m_window, monitor, 100, 100, m_width, m_height, GLFW_DONT_CARE);
        }

        // update delta
        auto currTime = (uint64_t) (glfwGetTime() * 1000.0);
        auto delta = currTime - prevTime;
        prevTime = currTime;

        processInput(m_window);

        // draw OpenGL
        callback(delta, m_width, m_height);

        // commit by swapping buffer
        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }
}
bool Window::is_key_pressed(Window::ButtonDirections key) {
    bool status, result;
    switch (key) {
        case Window::ButtonDirections::Up:
            return (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS) || (get_joystick_value(GLFW_JOYSTICK_1, 1) < -.5f);
        case Window::ButtonDirections::Down:
            return glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS || (get_joystick_value(GLFW_JOYSTICK_1, 1) > .5f);
        case Window::ButtonDirections::Left:
            return glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS || (get_joystick_value(GLFW_JOYSTICK_1, 0) > .5f);
        case Window::ButtonDirections::Right:
            return glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS || (get_joystick_value(GLFW_JOYSTICK_1, 0) < -.5f);
        case Window::ButtonDirections::Shoot:
            status = glfwGetKey(m_window, GLFW_KEY_E) == GLFW_PRESS || get_joystick_button(GLFW_JOYSTICK_1, 4);
            result = status ^ was_key_shoot_pressed && status;
            was_key_shoot_pressed = status;

            return result;
        case Window::ButtonDirections::Fly:
            status = glfwGetKey(m_window, GLFW_KEY_O) == GLFW_PRESS || get_joystick_button(GLFW_JOYSTICK_1, 2);
            result = status ^ was_key_fly_pressed && status;
            was_key_fly_pressed = status;

            return result;
        case Window::ButtonDirections::Debug:
            status = glfwGetKey(m_window, GLFW_KEY_I) == GLFW_PRESS || get_joystick_button(GLFW_JOYSTICK_1, 1);
            result = status ^ was_key_debug_pressed && status;
            was_key_debug_pressed = status;
            if (result)
                key_debug_status = !key_debug_status;

            return key_debug_status;
        case Window::ButtonDirections::Jump:
            return glfwGetKey(m_window, GLFW_KEY_SPACE) == GLFW_PRESS || get_joystick_button(GLFW_JOYSTICK_1, 0);
    }
}

bool Window::get_cursor_position(glm::vec2 &out_position) {
#ifndef VR_USE_ARROW_KEYS_AS_MOUSE
    double x, y;
    glfwGetCursorPos(m_window, &x, &y);

    float joy_x, joy_y;
    joy_x = get_joystick_value(GLFW_JOYSTICK_1, 2) * 1.5f;
    joy_y = get_joystick_value(GLFW_JOYSTICK_1, 3) * 1.5f;

    auto new_position = glm::vec2{(float) x, (float) y};
    auto did_change = new_position != m_mouse_position || abs(joy_x) >= .1f || abs(joy_y) >= .1f;
    out_position = new_position - m_mouse_position;

    out_position.x += joy_x;
    out_position.y += joy_y;

    m_mouse_position = new_position;

    return did_change;
#endif
#ifdef VR_USE_ARROW_KEYS_AS_MOUSE
    out_position.y += (glfwGetKey(m_window, GLFW_KEY_UP) == GLFW_PRESS) || (get_joystick_value(GLFW_JOYSTICK_1, 1) < -.5f) ? 1.f : 0.f;
    out_position.y += glfwGetKey(m_window, GLFW_KEY_DOWN) == GLFW_PRESS || (get_joystick_value(GLFW_JOYSTICK_1, 1) > .5f) ? -1.f : 0.f;
    out_position.x += glfwGetKey(m_window, GLFW_KEY_RIGHT) == GLFW_PRESS || (get_joystick_value(GLFW_JOYSTICK_1, 0) > .5f) ? 1.f : 0.f;
    out_position.x += glfwGetKey(m_window, GLFW_KEY_LEFT) == GLFW_PRESS || (get_joystick_value(GLFW_JOYSTICK_1, 0) < -.5f) ? -1.f : 0.f;

    out_position *= 3;

    return true;
#endif
}
bool Window::get_viewport(int &width, int &height) {
    width = m_width;
    height = m_height;

    auto hasChanged = hasViewportChanged;
    hasViewportChanged = false;
    return hasChanged;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
    hasViewportChanged = true;
    m_width = width;
    m_height = height;
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

float get_joystick_value(int glfw_joystick, unsigned int axis) {
    if (!glfwJoystickPresent(glfw_joystick))
        return 0.f;

    int count;
    const float *inputs = glfwGetJoystickAxes(glfw_joystick, &count);
    return count > axis ? *(inputs + axis) : 0.f;
}

bool get_joystick_button(int glfw_joystick, unsigned int btn) {
    if (!glfwJoystickPresent(glfw_joystick))
        return false;

    int count;
    const unsigned char *inputs = glfwGetJoystickButtons(glfw_joystick, &count);
    return count > btn && *(inputs + btn) == GLFW_PRESS;
}