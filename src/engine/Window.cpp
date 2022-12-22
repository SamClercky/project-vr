#include <iostream>

#include "Window.h"

using namespace engine;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
float get_joystick_value(int glfw_joystick, unsigned int axis);

static int m_width = 0.f;
static int m_height = 0.f;
static bool hasViewportChanged = true;

static bool debugMode = false;

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
void Window::loop(const std::function<void(const uint64_t)> &callback) {
    auto prevTime = (uint64_t) (glfwGetTime() * 1000.0);

    // vsync
    glfwSwapInterval(1);
    while (!glfwWindowShouldClose(m_window)) {
        // update delta
        auto currTime = (uint64_t) (glfwGetTime() * 1000.0);
        auto delta = currTime - prevTime;
        prevTime = currTime;

        processInput(m_window);

        // draw OpenGL
        callback(delta);

        // commit by swapping buffer
        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }
}
bool Window::is_key_pressed(Window::ButtonDirections key) {
    switch (key) {
        case Window::ButtonDirections::Up:
            return (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS) || (get_joystick_value(GLFW_JOYSTICK_1, 1) < -.5f);
        case Window::ButtonDirections::Down:
            return glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS || (get_joystick_value(GLFW_JOYSTICK_1, 1) > .5f);
        case Window::ButtonDirections::Left:
            return glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS || (get_joystick_value(GLFW_JOYSTICK_1, 0) > .5f);
        case Window::ButtonDirections::Right:
            return glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS || (get_joystick_value(GLFW_JOYSTICK_1, 0) < -.5f);
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

bool Window::is_debug_mode() {
#ifndef NDEBUG
    return debugMode;
#else
    return false;
#endif
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
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
        debugMode = !debugMode;
}

float get_joystick_value(int glfw_joystick, unsigned int axis) {
    if (!glfwJoystickPresent(glfw_joystick))
        return 0.f;

    int count;
    const float *inputs = glfwGetJoystickAxes(glfw_joystick, &count);
    return count > axis ? *(inputs + axis) : 0.f;
}
