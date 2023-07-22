#ifndef __WINDOW_H__
#define __WINDOW_H__

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Window {
private:
    GLFWwindow *window = nullptr;

public:
    Window(const char* title, uint32_t width, uint32_t height);
    ~Window() {
        glfwTerminate();
    }
    inline GLFWwindow* get_window() const { return this->window; }
    
    void close_window();
    void poll_events();
    void swap_buffers();
    bool is_key_pressed(int keycode);
    bool is_mouse_button_pressed(int button);
    glm::vec2 get_mouse_pos();
    glm::vec2 get_size();
};

#endif // __WINDOW_H__: header
