#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "gamedata.hpp"
#include "consts.hpp"
#include "vertex.hpp"

// glfw callback functions

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        glm::vec3 pos = { (float)x, (float)y, 1.0f };
        //translationA = pos; 
    }
}


GameData::GameData() {

    // initialize glfw and glew
    this->init();
    
    this->quad_buffer = new Vertex[MAX_VERTEX_COUNT]; // cpu buffer
    this->vb = new VertexBuffer(nullptr, MAX_VERTEX_COUNT * sizeof(Vertex));


    // vertex layout
    this->layout = new VertexBufferLayout();
    this->layout->Push<float>(2); // position
    this->layout->Push<float>(4); // color
    this->layout->Push<float>(2); // tex_coords
    this->layout->Push<float>(1); // tex_idx

    this->va = new VertexArray();
    this->va->AddBuffer(*this->vb, *this->layout);

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

    this->ib = new IndexBuffer(indices, MAX_IDX_COUNT);

    // view proj matrix
    this->proj = glm::ortho(0.0f, (float)WIDTH, 0.f, (float)HEIGHT, -1.0f, 1.0f);
    this->view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

    // load game data
    this->load_textures();
    this->load_shaders();
    this->va->Unbind();
    this->vb->Unbind();
    this->ib->Unbind();


    // set all glfw callback functions
    glfwSetKeyCallback(this->window, key_callback);
    glfwSetMouseButtonCallback(this->window, mouse_button_callback);
}

void GameData::load_textures() {
    std::cout << "Loading textures!\n";
    
    this->texture_map[0] = new Texture(""); // 0 = white tex
    this->texture_map[1] = new Texture("res/textures/grid_0-1.png");
    this->texture_map[2] = new Texture("res/textures/Octocat.png");

    for (uint32_t i = 0; i < 3; ++i) {
        this->texture_map[i]->Bind(i);
    }
}

void GameData::load_shaders() {
    std::cout << "Loading shaders!\n";
    
    this->shader_map[0] = new Shader("res/shaders/basic.shader");
    // 0 = white tex, 1, 2, 3... = tex
    int v[MAX_TEXTURES];
    for (uint32_t i = 0; i < MAX_TEXTURES; ++i) {
        v[i] = i;
    }
    
    // setup textures
    this->shader_map[0]->Bind();
    this->shader_map[0]->SetUinform1iv("u_textures", v, MAX_TEXTURES);
    this->shader_map[0]->Unbind();
}

GameData::~GameData() {
    glfwTerminate();
    delete this->va;
    delete this->vb;
    delete this->layout;
    delete this->ib;
    delete[] this->quad_buffer;

    for (uint32_t i = 0; i < this->texture_map.size(); ++i) {
        delete this->texture_map[i];
        this->texture_map.erase(i);
    }

    for (uint32_t i = 0; i < this->shader_map.size(); ++i) {
        delete this->shader_map[i];
        this->shader_map.erase(i);
    }
}

void GameData::begin_batch() {
    this->quad_buffer_ptr = this->quad_buffer;
}

void GameData::end_batch() {
    GLsizeiptr size = (uint8_t*)this->quad_buffer_ptr - (uint8_t*)this->quad_buffer;
    this->vb->Bind();
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, this->quad_buffer);
}

void GameData::flush() {
    this->va->Bind();
    glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, nullptr);    
  
    this->index_count = 0; // reset the amount of drew quads
}

void GameData::clear(glm::vec4 color) {
    glClearColor(color.x, color.y, color.z, color.w);
    glClear(GL_COLOR_BUFFER_BIT);
}

void GameData::init() {

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
    this->window = glfwCreateWindow(WIDTH, HEIGHT, "Waves", NULL, NULL);
    if (!this->window)
    {
        glfwTerminate();
        std::cout << "Could not glfw create window!\n";
        exit(1);
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(this->window);

    glfwSwapInterval(1);

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

void GameData::draw_quad(glm::vec2 pos, glm::vec2 size, float tex_idx) {

    if (this->index_count >= MAX_IDX_COUNT) {
        this->end_batch();
        this->flush();
        this->begin_batch();
    }
    
    this->quad_buffer_ptr->position = pos;
    this->quad_buffer_ptr->color = { 1.0f, 1.0f, 1.0f, 1.0f };
    this->quad_buffer_ptr->tex_coords = { 0.0f, 0.0f };
    this->quad_buffer_ptr->tex_idx = tex_idx;
    this->quad_buffer_ptr++;

    this->quad_buffer_ptr->position = { pos.x + size.x, pos.y };
    this->quad_buffer_ptr->color = { 1.0f, 1.0f, 1.0f, 1.0f };
    this->quad_buffer_ptr->tex_coords = { 1.0f, 0.0f };
    this->quad_buffer_ptr->tex_idx = tex_idx;
    this->quad_buffer_ptr++;

    this->quad_buffer_ptr->position = { pos.x + size.x, pos.y + size.y };
    this->quad_buffer_ptr->color = { 1.0f, 1.0f, 1.0f, 1.0f };
    this->quad_buffer_ptr->tex_coords = { 1.0f, 1.0f };
    this->quad_buffer_ptr->tex_idx = tex_idx;
    this->quad_buffer_ptr++;

    this->quad_buffer_ptr->position = { pos.x, pos.y + size.y };
    this->quad_buffer_ptr->color = { 1.0f, 1.0f, 1.0f, 1.0f };
    this->quad_buffer_ptr->tex_coords = { 0.0f, 1.0f };
    this->quad_buffer_ptr->tex_idx = tex_idx;
    this->quad_buffer_ptr++;

    this->index_count += 6; // each quad draw has 6 indexes;
}

void GameData::draw_quad(glm::vec2 pos, glm::vec2 size, glm::vec4 color) {
    
    if (this->index_count >= MAX_IDX_COUNT) {
        this->end_batch();
        this->flush();
        this->begin_batch();
    }
   
    this->quad_buffer_ptr->position = pos;
    this->quad_buffer_ptr->color = color;
    this->quad_buffer_ptr->tex_coords = { 0.0f, 0.0f };
    this->quad_buffer_ptr->tex_idx = 0.0f;
    this->quad_buffer_ptr++;

    this->quad_buffer_ptr->position = { pos.x + size.x, pos.y };
    this->quad_buffer_ptr->color = color;
    this->quad_buffer_ptr->tex_coords = { 1.0f, 0.0f };
    this->quad_buffer_ptr->tex_idx = 0.0f;
    this->quad_buffer_ptr++;

    this->quad_buffer_ptr->position = { pos.x + size.x, pos.y + size.y };
    this->quad_buffer_ptr->color = color;
    this->quad_buffer_ptr->tex_coords = { 1.0f, 1.0f };
    this->quad_buffer_ptr->tex_idx = 0.0f;
    this->quad_buffer_ptr++;

    this->quad_buffer_ptr->position = { pos.x, pos.y + size.y };
    this->quad_buffer_ptr->color = color;
    this->quad_buffer_ptr->tex_coords = { 0.0f, 1.0f };
    this->quad_buffer_ptr->tex_idx = 0.0f;
    this->quad_buffer_ptr++;

    this->index_count += 6; // each quad draw has 6 indexes;
}
