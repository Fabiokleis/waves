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

struct GameData {
    glm::mat4 proj;
    glm::mat4 view;
    
    VertexArray *va = nullptr;
    VertexBuffer *vb = nullptr;
    Vertex *quad_buffer = nullptr;
    Vertex *quad_buffer_ptr = nullptr;
    VertexBufferLayout *layout = nullptr;
    IndexBuffer *ib = nullptr;
    uint32_t index_count = 0;
    
    std::unordered_map<uint32_t, Texture*> texture_map;
    std::unordered_map<uint32_t, Shader*> shader_map;
};

static GameData gd; 


void Renderer::set_camera(glm::mat4 p, glm::mat4 v) {
    gd.proj = p;
    gd.view = v;
}

glm::mat4 Renderer::get_camera() {
    return gd.proj * gd.view;
}

Shader* Renderer::get_shader(uint32_t shader_idx) {
    return gd.shader_map[shader_idx];
}

glm::vec2 Renderer::get_texture_size(uint32_t tex_idx) {
    int width = gd.texture_map[tex_idx]->GetWidth();
    int height = gd.texture_map[tex_idx]->GetHeight();
    return glm::vec2(width, height);
}

Renderer::Renderer() {

    // initialize glfw and glew
    this->init();   
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
    // load game data
    load_textures();
    load_shaders();
    gd.va->Unbind();
    gd.vb->Unbind();
    gd.ib->Unbind();
}

// just initialize glew
void Renderer::init() {
    std::cout << "Initializing glew!\n";
    unsigned int err = glewInit();
    if (GLEW_OK != err) {
        std::cout << "GLEW initialization error!\n";
    }
    
    std::cout << glGetString(GL_VERSION) << "\n";
    std::cout << glGetString(GL_RENDERER) << "\n";

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Renderer::load_textures() {
    std::cout << "Loading textures!\n";
    
    gd.texture_map[0] = new Texture(""); // 0 = white tex
    gd.texture_map[1] = new Texture("res/textures/grid_0-1.png");
    gd.texture_map[2] = new Texture("res/textures/Octocat.png");
    gd.texture_map[3] = new Texture("res/textures/32x32-bat-sprite.png");

    for (uint32_t i = 0; i < 4; ++i) {
        gd.texture_map[i]->Bind(i);
    }
}

void Renderer::load_shaders() {
    std::cout << "Loading shaders!\n";
    
    gd.shader_map[0] = new Shader("res/shaders/shader.vert", "res/shaders/shader.frag");
    
    // 0 = white tex, 1, 2, 3... = tex
    int v[MAX_TEXTURES];
    for (uint32_t i = 0; i < MAX_TEXTURES; ++i) {
        v[i] = i;
    }
    
    // setup textures
    gd.shader_map[0]->Bind();
    gd.shader_map[0]->SetUinform1iv("u_textures", v, MAX_TEXTURES);
    gd.shader_map[0]->Unbind();
}

Renderer::~Renderer() {
    delete gd.va;
    delete gd.vb;
    delete gd.layout;
    delete gd.ib;
    delete[] gd.quad_buffer;

    for (uint32_t i = 0; i < gd.texture_map.size(); ++i) {
        delete gd.texture_map[i];
        gd.texture_map.erase(i);
    }

    for (uint32_t i = 0; i < gd.shader_map.size(); ++i) {
        delete gd.shader_map[i];
        gd.shader_map.erase(i);
    }
}

void Renderer::begin_batch() {
    gd.quad_buffer_ptr = gd.quad_buffer;
}

void Renderer::end_batch() {
    GLsizeiptr size = (uint8_t*)gd.quad_buffer_ptr - (uint8_t*)gd.quad_buffer;
    gd.vb->Bind();
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, gd.quad_buffer);
}

void Renderer::flush() {
    gd.va->Bind();
    glDrawElements(GL_TRIANGLES, gd.index_count, GL_UNSIGNED_INT, nullptr);    
  
    gd.index_count = 0; // reset the amount of drew quads
}

void Renderer::clear(glm::vec4 color) {
    glClearColor(color.x, color.y, color.z, color.w);
    glClear(GL_COLOR_BUFFER_BIT);
}

static void append_vertex(glm::vec2 pos, glm::vec2 size, glm::vec2 tex_coord, glm::vec4 color, float tex_idx) {
    gd.quad_buffer_ptr->position = pos;
    gd.quad_buffer_ptr->color = color;
    gd.quad_buffer_ptr->tex_coords = tex_coord;
    gd.quad_buffer_ptr->tex_idx = tex_idx;
    gd.quad_buffer_ptr++;
}

// texture uv
void Renderer::draw_quad(glm::vec2 pos, glm::vec2 size, glm::vec4 uv, uint32_t tex_idx) {
    
    if (gd.index_count >= MAX_IDX_COUNT) {
        Renderer::end_batch();
        Renderer::flush();
        Renderer::begin_batch();
    }
    
    tex_idx = (float) tex_idx;

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
void Renderer::draw_quad(glm::vec2 pos, glm::vec2 size, glm::vec4 color) {
    
    if (gd.index_count >= MAX_IDX_COUNT) {
        Renderer::end_batch();
        Renderer::flush();
        Renderer::begin_batch();
    }
    
    append_vertex(pos, size, glm::vec2(0.0f, 0.0f), color, 0.0f);
    append_vertex(glm::vec2(pos.x + size.x, pos.y), size, glm::vec2(1.0f, 0.0f), color, 0.0f);
    append_vertex(glm::vec2(pos.x + size.x, pos.y + size.y), size, glm::vec2(1.0f, 1.0f), color, 0.0f);
    append_vertex(glm::vec2(pos.x, pos.y + size.y), size, glm::vec2(0.0f, 1.0f), color, 0.0f);

    gd.index_count += 6; // each quad draw has 6 indexes;
}
