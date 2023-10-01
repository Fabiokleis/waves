#ifndef __RENDERER_H__
#define __RENDERER_H__

#include <glm/glm.hpp>
#include "texture.hpp"
#include "shader.hpp"
#include "vertex.hpp"
#include "vertexarray.hpp"
#include "vertexbuffer.hpp"
#include "vertexbufferlayout.hpp"
#include "indexbuffer.hpp"

#include <string>
#include <unordered_map>


class Renderer
{
private:

    struct TexIdx {
	uint32_t idx;
	Texture *tex = nullptr;
    };
    
    struct GameData {
    
        VertexArray *va = nullptr;
        VertexBuffer *vb = nullptr;
        Vertex *quad_buffer = nullptr;
        Vertex *quad_buffer_ptr = nullptr;
        VertexBufferLayout *layout = nullptr;
        IndexBuffer *ib = nullptr;
        uint32_t index_count = 0;
    
        uint32_t texture_indexes = 0;
        uint32_t shader_indexes = 0;

        std::unordered_map<std::string, TexIdx> texture_map;
        std::unordered_map<uint32_t, Shader*> shader_map;
    };

    GameData gd;


    
public:

    Renderer(const Renderer &obj) = delete;
    
    static Renderer* get() {
        if (instance == nullptr) {
            instance = new Renderer();
            return instance;
        } else {
            return instance;
        }
    }
    
    ~Renderer();

    static void init() { get()->init_impl(); }
    static void clear(glm::vec4 color) { get()->clear_impl(color); }
    static void begin_batch() { get()->begin_batch_impl(); }
    static void end_batch() { get()->end_batch_impl(); }
    static void flush() { get()->flush_impl(); }

    static void load_texture(const std::string& key, const std::string& path) { get()->load_texture_impl(key, path); }
    
    static void load_shader(const std::string& vert_path, const std::string& frag_path) {
        get()->load_shader_impl(vert_path, frag_path);
    }
    
    static Shader* get_shader(uint32_t shader_idx) { return get()->get_shader_impl(shader_idx); }

    static glm::vec2 get_texture_size(const std::string &key) { return get()->get_texture_size_impl(key); }
    
    static void draw_quad(glm::vec2 pos, glm::vec2 size, glm::vec4 uv, const std::string& key) {
        get()->draw_quad_impl(pos, size, uv, key);
    }
    
    static void draw_quad(glm::vec2 pos, glm::vec2 size, glm::vec4 color) {
        get()->draw_quad_impl(pos, size, color);
    }

    
private:
    Renderer() {}
    static Renderer *instance;

    void init_impl();
    void clear_impl(glm::vec4 color);
    void begin_batch_impl();
    void end_batch_impl();
    void flush_impl();
    void load_texture_impl(const std::string& key, const std::string& path);
    void load_shader_impl(const std::string& vert_path, const std::string& frag_path);
    Shader* get_shader_impl(uint32_t shader_idx);
    glm::vec2 get_texture_size_impl(const std::string& key);
    void draw_quad_impl(glm::vec2 pos, glm::vec2 size, glm::vec4 uv, const std::string& key);
    void draw_quad_impl(glm::vec2 pos, glm::vec2 size, glm::vec4 color);

    void append_vertex(glm::vec2 pos, glm::vec2 size, glm::vec2 tex_coord, glm::vec4 color, float tex_idx);
};



#endif // __RENDERER_H__: header 
