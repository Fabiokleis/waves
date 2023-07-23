#include <iostream>
#include <GLFW/glfw3.h>
#include "window.hpp"

Window::Window(const char* title, uint32_t width, uint32_t height) {
    std::cout << "Initializing glfw!\n";

    if (!glfwInit()) {
        std::cout << "Could not initialize glfw!\n";
        exit(1);
    }

    glfwInitHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwInitHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwInitHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);

    std::cout << "Creating window!\n";
    /* Create a windowed mode window and its OpenGL context */
    this->window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!this->window)
    {
        glfwTerminate();
        std::cout << "Could not create glfw window!\n";
        exit(1);
    }
    /* Make the window's context current */
    glfwMakeContextCurrent(this->window);
    glfwSwapInterval(1);
}

void Window::close_window() {
    glfwSetWindowShouldClose(this->window, GLFW_TRUE);
}

void Window::poll_events() {
    glfwPollEvents();
}

void Window::swap_buffers() {
    glfwSwapBuffers(this->window);
}

bool Window::is_key_pressed(int keycode) const {
    int state = glfwGetKey(this->window, keycode);
    return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool Window::is_mouse_button_pressed(int button) const {
    int state = glfwGetMouseButton(this->window, button);
    return state == GLFW_PRESS;
}

glm::vec2 Window::get_mouse_pos() {
    double xpos, ypos;
    glfwGetCursorPos(this->window, &xpos, &ypos);

    return glm::vec2((float)xpos, (float)ypos);
}

glm::vec2 Window::get_size() {
    int w, h;
    glfwGetWindowSize(this->window, &w, &h);

    return glm::vec2(w, h);
}
