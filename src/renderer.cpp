#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "renderer.hpp"
#include "consts.hpp"
#include "texture.hpp"
#include "shader.hpp"
#include "vertex.hpp"
#include "vertexarray.hpp"
#include "vertexbuffer.hpp"
#include "vertexbufferlayout.hpp"
#include "indexbuffer.hpp"

#include <unordered_map>

Renderer* Renderer::instance = nullptr;

Shader* Renderer::get_shader_impl(uint32_t shader_idx) {
    return gd.shader_map[shader_idx];
}

glm::vec2 Renderer::get_texture_size_impl(const std::string& key) {
    int width = gd.texture_map[key].tex->GetWidth();
    int height = gd.texture_map[key].tex->GetHeight();
    return glm::vec2(width, height);
}

void Renderer::init_impl() {

    std::cout << "Initializing glew!\n";
    unsigned int err = glewInit();
    if (GLEW_OK != err) {
        std::cout << "GLEW initialization error!\n";
    }
    
    std::cout << glGetString(GL_VERSION) << "\n";
    std::cout << glGetString(GL_RENDERER) << "\n";

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    gd.quad_buffer = new Vertex[MAX_VERTEX_COUNT]; // cpu buffer
    gd.vb = new VertexBuffer(nullptr, MAX_VERTEX_COUNT * sizeof(Vertex));

    // vertex layout
    gd.layout = new VertexBufferLayout();
    gd.layout->Push<float>(2); // position
    gd.layout->Push<float>(4); // color
    gd.layout->Push<float>(2); // tex_coords
    gd.layout->Push<float>(1); // tex_idx

    gd.va = new VertexArray();
    gd.va->AddBuffer(*gd.vb, *gd.layout);

    // populate all triangles indexes
    uint32_t indices[MAX_IDX_COUNT];
    uint32_t offset = 0;
    for (size_t i = 0; i < MAX_IDX_COUNT; i+=6) {
        indices[i + 0] = 0 + offset;
        indices[i + 1] = 1 + offset;
        indices[i + 2] = 2 + offset;
        
        indices[i + 3] = 2 + offset;
        indices[i + 4] = 3 + offset;
        indices[i + 5] = 0 + offset;

        offset += 4;
    }

    gd.ib = new IndexBuffer(indices, MAX_IDX_COUNT);
}

void Renderer::load_texture_impl(const std::string &key, const std::string &path) {
    gd.texture_map[key] = (TexIdx){ .idx = gd.texture_indexes, .tex = new Texture(path) };
    gd.texture_map[key].tex->Bind(gd.texture_indexes++);

    for (auto i : gd.texture_map) {
	i.second.tex->Bind(i.second.idx);
    }
}

void Renderer::load_shader_impl(const std::string& vert_path, const std::string& frag_path) {
    gd.shader_map[gd.shader_indexes] = new Shader(vert_path, frag_path);
    // 0 = white tex, 1, 2, 3... = tex
    int v[MAX_TEXTURES];
    for (uint32_t i = 0; i < MAX_TEXTURES; ++i) {
        v[i] = i;
    }
    // setup textures
    gd.shader_map[gd.shader_indexes]->Bind();
    gd.shader_map[gd.shader_indexes]->SetUinform1iv("u_textures", v, MAX_TEXTURES);
    gd.shader_map[gd.shader_indexes]->Unbind();
    gd.shader_indexes++;
}

Renderer::~Renderer() {
    delete gd.va;
    delete gd.vb;
    delete gd.layout;
    delete gd.ib;
    delete[] gd.quad_buffer;

    for (auto i = gd.texture_map.begin(); i != gd.texture_map.end(); i++) {
        delete i->second.tex;
        gd.texture_map.erase(i);
    }

    for (uint32_t i = 0; i < gd.shader_map.size(); ++i) {
        delete gd.shader_map[i];
        gd.shader_map.erase(i);
    }

    delete instance;
}

void Renderer::begin_batch_impl() {
    gd.quad_buffer_ptr = gd.quad_buffer;
}

void Renderer::end_batch_impl() {
    GLsizeiptr size = (uint8_t*)gd.quad_buffer_ptr - (uint8_t*)gd.quad_buffer;
    gd.vb->Bind();
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, gd.quad_buffer);
}

void Renderer::flush_impl() {
    gd.va->Bind();
    glDrawElements(GL_TRIANGLES, gd.index_count, GL_UNSIGNED_INT, nullptr);    
  
    gd.index_count = 0; // reset the amount of drew quads
}

void Renderer::clear_impl(glm::vec4 color) {
    glClearColor(color.x, color.y, color.z, color.w);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::append_vertex(glm::vec2 pos, glm::vec2 size, glm::vec2 tex_coord, glm::vec4 color, float tex_idx) {
    gd.quad_buffer_ptr->position = pos;
    gd.quad_buffer_ptr->color = color;
    gd.quad_buffer_ptr->tex_coords = tex_coord;
    gd.quad_buffer_ptr->tex_idx = tex_idx;
    gd.quad_buffer_ptr++;
}

// texture uv
void Renderer::draw_quad_impl(glm::vec2 pos, glm::vec2 size, glm::vec4 uv, const std::string& key) {
    
    if (gd.index_count >= MAX_IDX_COUNT) {
        Renderer::end_batch_impl();
        Renderer::flush_impl();
        Renderer::begin_batch_impl();
    }
    
    float tex_idx = (float) gd.texture_map[key].idx;

    glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
    glm::vec2 top_left = { uv.x, uv.y  };
    glm::vec2 top_right = { uv.z, uv.y };
    glm::vec2 bottom_right = { uv.z, uv.w };
    glm::vec2 bottom_left = { uv.x, uv.w };
    
    append_vertex(pos, size, top_left, color, tex_idx);
    append_vertex(glm::vec2(pos.x + size.x, pos.y), size, top_right, color, tex_idx);
    append_vertex(glm::vec2(pos.x + size.x, pos.y + size.y), size, bottom_right, color, tex_idx);
    append_vertex(glm::vec2(pos.x, pos.y + size.y), size, bottom_left, color, tex_idx);
    
    gd.index_count += 6;
}

// color fill
void Renderer::draw_quad_impl(glm::vec2 pos, glm::vec2 size, glm::vec4 color) {
    
    if (gd.index_count >= MAX_IDX_COUNT) {
        Renderer::end_batch_impl();
        Renderer::flush_impl();
        Renderer::begin_batch_impl();
    }
    
    append_vertex(pos, size, glm::vec2(0.0f, 0.0f), color, 0.0f);
    append_vertex(glm::vec2(pos.x + size.x, pos.y), size, glm::vec2(1.0f, 0.0f), color, 0.0f);
    append_vertex(glm::vec2(pos.x + size.x, pos.y + size.y), size, glm::vec2(1.0f, 1.0f), color, 0.0f);
    append_vertex(glm::vec2(pos.x, pos.y + size.y), size, glm::vec2(0.0f, 1.0f), color, 0.0f);

    gd.index_count += 6; // each quad draw has 6 indexes;
}
