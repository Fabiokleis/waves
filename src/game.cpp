#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "game.hpp"
#include "consts.hpp"
#include "renderer.hpp"
#include "projectile.hpp"

using namespace Waves;

Game::Game() {
    init();
}

Game::~Game() {
    if (window)
        delete window;

    if (player)
        delete player;

    for (auto proj : projectiles) {
        delete proj;
    }
    projectiles.clear();
}

void Game::run() {

    total_time = 0.0f;
    start_time = glfwGetTime();
    current_time = 0;
    delta_time = 0;

    while (!quit) {
        total_time += (float)delta_time;
        window->poll_events();


        std::cout << projectiles.size() << "\n";
        
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

void Game::sync_player_camera() {
    view = glm::translate(glm::mat4(1.f), player->camera_offset);
    player->camera = proj * view;
}

void Game::update() {
    quit = window->is_key_pressed(GLFW_KEY_ESCAPE);
    player->update(*window, (float)delta_time);
    if (player->throw_projectile(*window)) {
        float angle = glm::radians(player->get_rotation());
        float scale = player->get_scale();
        glm::vec2 p_pos = player->get_position();
        glm::vec2 p_size = player->get_size();
        glm::vec2 pos = glm::vec2(
                                  p_pos.x - (p_size.x/2) - (p_size.x/2 * scale),
                                  p_pos.y - (p_size.y/2) - (p_size.y/2 * scale));
                                  
        glm::vec2 vel = glm::vec2(
                                  PROJECTILE_VELOCITY * glm::cos(angle),
                                  PROJECTILE_VELOCITY * glm::sin(angle));
        
        uint32_t texture_idx = 2;
        uint32_t cell_width = 16;
        uint32_t cell_height = 16;
        uint32_t rows = 1;
        uint32_t cols = 1;

        projectiles.push_back(new Projectile(pos, vel, texture_idx, cell_width, cell_height, rows, cols));
    }

    for (uint32_t i = 0; i < projectiles.size(); ++i) {
        if (projectiles[i]->ded)
            projectiles.erase(projectiles.begin() + i);
        else
            projectiles[i]->update(*window, delta_time);
    }
    
    sync_player_camera();
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

        for (auto proj : projectiles) {
            proj->draw();
        }
        
    }
    Renderer::end_batch();                                                                
    Renderer::flush();        
    
    player->draw();
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
                              3, 32, 32, 1, 4); // tex_idx, tile width, tile height, rows, cols
    
    // view proj matrix
    translation = glm::vec3(0.f);
    proj = glm::ortho(0.0f, (float)WIDTH, 0.f, (float)HEIGHT, -1.0f, 1.0f);
    view = glm::translate(glm::mat4(1.0f), translation);

    player->camera_offset = translation;
    player->camera = proj * view;
}

void Game::load_textures() {
    Renderer::load_texture("");
    Renderer::load_texture("res/textures/32x32-bat-sprite.png");
    Renderer::load_texture("res/textures/clay2.png");
    Renderer::load_texture("res/textures/player.png");
}

void Game::load_shaders() {
    Renderer::load_shader("res/shaders/shader.vert", "res/shaders/shader.frag");
}
