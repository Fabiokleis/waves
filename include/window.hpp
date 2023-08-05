#ifndef __WINDOW_H__
#define __WINDOW_H__

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <string>

class Window {
private:
    GLFWwindow *window = nullptr;
    struct CustomCursor {
        GLFWcursor *glfw_cursor = nullptr;
        unsigned char *image_buffer = nullptr;
        int channels_in_file;
        int i_width;
        int i_height;
    };

    CustomCursor cursor; 
                  

public:
    Window(const std::string& title, uint32_t width, uint32_t height);
    ~Window();
    inline GLFWwindow* get_window() const { return this->window; }
    
    void close_window();
    void poll_events();
    void swap_buffers();
    void set_custom_cursor_image(const std::string &icon_path);
    bool is_key_pressed(int keycode) const;
    bool is_mouse_button_pressed(int button) const;
    bool is_mouse_button_released(int button) const;
    bool window_should_close();
    glm::vec2 get_mouse_pos() const;
    glm::vec2 get_size() const;
};

#endif // __WINDOW_H__: header
