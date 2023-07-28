#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "game.hpp"
#include "consts.hpp"
#include "renderer.hpp"

using namespace Waves;

Game::Game() {
    init();
}

Game::~Game() {
    if (window)
        delete window;

    if (player)
        delete player;
}

void Game::run() {

    total_time = 0.0f;
    start_time = glfwGetTime();
    current_time = 0;
    delta_time = 0;

    while (!quit) {
        total_time += (float)delta_time;
        window->poll_events();


        // update entities
        update();

        // draw game
        draw();
        window->swap_buffers();


        // update time
        current_time = glfwGetTime();
        float wt = (FRAME_TIME) - (current_time - start_time);
        if (wt > 0.0 && wt <= FRAME_TIME) {
            delta_time = current_time - start_time;
        }
        start_time = current_time;
        
    }
}

void Game::update() {
    player->update(*window, (float)delta_time);
    quit = window->is_key_pressed(GLFW_KEY_ESCAPE);
}

void Game::draw() {

    Renderer::clear(glm::vec4(0.0, 0.0f, 0.0, 1.0));
    
    // map
    Renderer::begin_batch();                                                              
    {                                                                                     
        glm::mat4 model = glm::mat4(1.0f);
        Renderer::get_shader(0)->Bind();                                                  
        Renderer::get_shader(0)->SetUniformMat4f("u_camera", proj * view);
        Renderer::get_shader(0)->SetUniformMat4f("u_model", model);
        Renderer::get_shader(0)->SetUniform4f("u_color", 0.5, 1, 1, 1.0f);                
        Renderer::get_shader(0)->SetUniform1f("u_time", total_time);                           
                                                                                              
        for (int y = 0; y < 20; ++y) {                                                    
            for (int x = 0; x < 20; ++x) {                                                
                glm::vec4 color = { (x + 10) / 20.0f, 0.6, (y + 10) / 20.0f, 1.0f };      
                Renderer::draw_quad(glm::vec2(x * 60, y * 60), glm::vec2(50, 50), color); 
            }                                                                             
        }                                                                                 
    }                                                                                     
    Renderer::end_batch();                                                                
    Renderer::flush();

    Renderer::begin_batch();                                                              
    {                                                                                     
        glm::mat4 model = glm::mat4(1.0f);
        Renderer::get_shader(0)->Bind();                                                  
        Renderer::get_shader(0)->SetUniformMat4f("u_camera", proj * view);
        Renderer::get_shader(0)->SetUniformMat4f("u_model", model);
        Renderer::get_shader(0)->SetUniform4f("u_color", 0.5, 1, 1, 1.0f);                
        Renderer::get_shader(0)->SetUniform1f("u_time", total_time);                           

        Renderer::draw_quad(glm::vec2(WIDTH/2, HEIGHT/2), glm::vec2(50, 50), glm::vec4(1.f, 0.f, 0.f, 1.f));
    }                                                                                     
    Renderer::end_batch();                                                                
    Renderer::flush();     

    player->draw(proj * view);
}

void Game::init() {
    window = new Window("Waves", WIDTH, HEIGHT);
    window->set_custom_cursor_image("res/textures/mouse_icon.png");
    Renderer::init();
    load_textures();
    load_shaders();

    // creating player with bat sprite sheet
    player = new Player(glm::vec2(100, 100), // world position
                              glm::vec2(200, 200),
                              1, 32, 32, 4, 4); // tex_idx, tile width, tile height, rows, cols
    
    // view proj matrix
    proj = glm::ortho(0.0f, (float)WIDTH, 0.f, (float)HEIGHT, -1.0f, 1.0f);
    view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

}

void Game::load_textures() {
    Renderer::load_texture(""); // 0 = white tex
    Renderer::load_texture("res/textures/32x32-bat-sprite.png");
    Renderer::load_texture("res/textures/clay_stone.png");
}

void Game::load_shaders() {
    Renderer::load_shader("res/shaders/shader.vert", "res/shaders/shader.frag");
}
