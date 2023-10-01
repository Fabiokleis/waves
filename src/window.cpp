#include <iostream>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <string>
#include "window.hpp"

Window::Window(const std::string &title, uint32_t width, uint32_t height) {
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
    this->window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
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

Window::~Window() {
    if (this->cursor.glfw_cursor != nullptr)
        glfwDestroyCursor(this->cursor.glfw_cursor);
    
    glfwTerminate();
}

bool Window::window_should_close() {
    return glfwWindowShouldClose(this->window);
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

void Window::set_custom_cursor_image(const std::string &icon_path) {
    
    this->cursor.image_buffer = stbi_load(
                                     icon_path.c_str(),
                                     &this->cursor.i_width,
                                     &this->cursor.i_height,
                                     &this->cursor.channels_in_file, 4);

    GLFWimage image;
    image.width = this->cursor.i_width;
    image.height = this->cursor.i_height;
    image.pixels = this->cursor.image_buffer;
    
    this->cursor.glfw_cursor = glfwCreateCursor(&image, 0, 0);
    if (NULL == this->cursor.glfw_cursor) {
        std::cerr << "Could not create a cursor." << std::endl;
        exit(1);
    }
    if (this->cursor.image_buffer) {
        stbi_image_free(this->cursor.image_buffer);
    }

    glfwSetCursor(window, this->cursor.glfw_cursor);
}

bool Window::is_key_pressed(int keycode) const {
    int state = glfwGetKey(this->window, keycode);
    return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool Window::is_mouse_button_pressed(int button) const {
    int state = glfwGetMouseButton(this->window, button);
    return state == GLFW_PRESS;
}

bool Window::is_mouse_button_released(int button) const {
    int state = glfwGetMouseButton(this->window, button);
    return state == GLFW_RELEASE;
}

glm::vec2 Window::get_mouse_pos() const {
    double xpos, ypos;
    glfwGetCursorPos(this->window, &xpos, &ypos);

    return glm::vec2((float)xpos, (float)ypos);
}

glm::vec2 Window::get_size() const {
    int w, h;
    glfwGetWindowSize(this->window, &w, &h);

    return glm::vec2(w, h);
}
