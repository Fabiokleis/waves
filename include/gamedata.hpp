#ifndef __GAMEDATA_H__
#define __GAMEDATA_H__

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <unordered_map>

#include "vertexbuffer.hpp"
#include "indexbuffer.hpp"
#include "vertexarray.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "vertex.hpp"
#include "spritesheet.hpp"

// should be instanciated after glfw context
class GameData
{
public:
    glm::mat4 proj;
    glm::mat4 view;
    glm::mat4 model;
    
    VertexArray *va = nullptr;
    VertexBuffer *vb = nullptr;
    Vertex *quad_buffer = nullptr;
    Vertex *quad_buffer_ptr = nullptr;
    VertexBufferLayout *layout = nullptr;
    IndexBuffer *ib = nullptr;
    uint32_t index_count = 0;
    Renderer renderer;
    
    std::unordered_map<uint32_t, Texture*> texture_map;
    std::unordered_map<uint32_t, Shader*> shader_map;

    GameData();
    ~GameData();
    
public:
    void clear(glm::vec4 color);
    void begin_batch();
    void end_batch();
    void flush();
    
    void draw_quad(glm::vec2 pos, glm::vec2 size, float tex_idx);
    void draw_quad(glm::vec2 pos, glm::vec2 size, glm::vec4 color);
    void draw_quad(glm::vec2 pos, const SpriteSheet& sheet, uint32_t row, uint32_t col, bool flip);
    
private:
    void init();
    void load_textures();
    void load_shaders();
    void append_vertex(glm::vec2 pos, glm::vec2 size, glm::vec2 tex_coord, glm::vec4 color, float tex_idx);
    glm::vec4 calculate_sprite_position(const SpriteSheet &sheet, uint32_t row, uint32_t col);
};



#endif // __GAMEDATA_H__: header 
