#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <array>
#include <string.h>
#include "renderer.hpp"
#include "vertexbuffer.hpp"
#include "indexbuffer.hpp"
#include "vertexarray.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "vertex.hpp"

#define WIDTH 1200
#define HEIGHT 720

glm::vec3 translationA(0, 0, 0);

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

static Vertex* CreateQuad(Vertex *buffer, float x, float y, float tex_idx) 
{
    float size = HEIGHT/5.0f;
    
    buffer->position = { x, y };
    buffer->color = { 0.0f, 0.0f, 1.0f, 1.0f };
    buffer->tex_coords = { 0.0f, 0.0f };
    buffer->tex_idx = tex_idx;
    buffer++;

    buffer->position = { x + size, y };
    buffer->color = { 0.0f, 0.0f, 1.0f, 1.0f };
    buffer->tex_coords = { 1.0f, 0.0f };
    buffer->tex_idx = tex_idx;
    buffer++;

    buffer->position = { x + size, y + size };
    buffer->color = { 0.0f, 0.0f, 1.0f, 1.0f };
    buffer->tex_coords = { 1.0f, 1.0f };
    buffer->tex_idx = tex_idx;
    buffer++;

    buffer->position = { x, y + size };
    buffer->color = { 0.0f, 0.0f, 1.0f, 0.0f };
    buffer->tex_coords = { 0.0f, 1.0f };
    buffer->tex_idx = tex_idx;
    buffer++;

    return buffer;
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;
   
    glfwInitHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwInitHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwInitHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    //const GLFWvidmode* mode = glfwGetVideoMode(monitor);

    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(WIDTH, HEIGHT, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    unsigned int err = glewInit();
    if (GLEW_OK != err) {
        std::cout << "Error!" << std::endl;
    }

    std::cout << glGetString(GL_VERSION) << std::endl;
    std::cout << glGetString(GL_RENDERER) << std::endl;
    std::cout << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    const size_t max_quad_count = 1000;
    const size_t max_vertex_count = max_quad_count * 4; // 4 vertex per quad
    const size_t max_idx_count = max_quad_count * 6; // 6 indexes per quad

    VertexArray va;
    VertexBuffer vb(nullptr, max_vertex_count * sizeof(Vertex));
    VertexBufferLayout layout;

    // vertex layout
    layout.Push<float>(2); // position
    layout.Push<float>(4); // color
    layout.Push<float>(2); // tex_coords
    layout.Push<float>(1); // tex_idx

    va.AddBuffer(vb, layout);

    uint32_t indices[max_idx_count];
    uint32_t offset = 0;
    for (size_t i = 0; i < max_idx_count; i+=6) {
        indices[i + 0] = 0 + offset;
        indices[i + 1] = 1 + offset;
        indices[i + 2] = 2 + offset;
        
        indices[i + 3] = 2 + offset;
        indices[i + 4] = 3 + offset;
        indices[i + 5] = 0 + offset;

        offset += 4;
    }
    
    IndexBuffer ib(indices, max_idx_count);

    glm::mat4 proj = glm::ortho(0.0f, (float)WIDTH, 0.f, (float)HEIGHT, -1.0f, 1.0f);
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

    Shader shader("res/shaders/basic.shader");
    shader.Bind();

    shader.SetUniform4f("u_color", 0.0f, 0.0f, 1.0f, 0.5f);
    
    Texture texture("res/textures/grid_0-1.png");
    Texture texture1("res/textures/Octocat.png");
    texture.Bind(1);
    texture1.Bind(2);
   

    // 0 = color, 1,2 = tex
    int v[3] = { 0, 1, 2 };
    shader.SetUinform1iv("u_textures", v, 3);
    va.Unbind();
    vb.Unbind();
    ib.Unbind();
    shader.Unbind();

    Renderer renderer;


    float r = 0.0f;
    float a = 0.1f;
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        // set dynamic vertex buffer
         
        std::array<Vertex, 1000> vertices;
        Vertex *buffer = vertices.data();
        
        for (int y = 0; y < 5; ++y) {
            for (int x = 0; x < 5; ++x) {
                buffer = CreateQuad(buffer, x * HEIGHT/5, y * HEIGHT/5, (x + y) % 2 + 1);
            }
        }
        
        vb.Bind();
        glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());
        
        a = r > 1.0f ? -0.05f : (r < 0.0f ? 0.05f : a);
        r += a;
        /* Render here */
        renderer.Clear();
        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA);
            glm::mat4 mvp = proj * view * model;
            shader.Bind();

            shader.SetUniform4f("u_color", r, 0.2f, 0.7f, 1.0f);
            shader.SetUniformMat4f("u_MVP", mvp);
            renderer.Draw(va, ib, shader);
        }
        
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }


    glfwTerminate();
    return 0;
}
