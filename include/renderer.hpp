#ifndef __RENDERER_H__
#define __RENDERER_H__

#include <glm/glm.hpp>
#include "shader.hpp"

class Renderer
{
public:
    Renderer();
    ~Renderer();
    
    static void clear(glm::vec4 color);
    static void begin_batch();
    static void end_batch();
    static void flush();

    static Shader* get_shader(uint32_t shader_idx);

    static void set_camera(glm::mat4 p, glm::mat4 v);
    static glm::mat4 get_camera();
    static glm::vec2 get_texture_size(uint32_t tex_idx);
    
    static void draw_quad(glm::vec2 pos, glm::vec2 size, glm::vec4 uv, uint32_t tex_idx);
    static void draw_quad(glm::vec2 pos, glm::vec2 size, glm::vec4 color);
        
private:
    void init();
    void load_textures();
    void load_shaders();
};

#endif // __RENDERER_H__: header 
