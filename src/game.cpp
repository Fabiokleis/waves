#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "game.hpp"
#include "player.hpp"
#include "consts.hpp"
#include "renderer.hpp"
#include "projectile.hpp"
#include "map.hpp"
#include <cstdlib>
#include <iostream>

using namespace Waves;

Game::Game() {
    init();
}

Game::~Game() {
    if (window)
        delete window;

    if (player)
        delete player;

    if (tile_map)
	delete tile_map;
	    
    for (auto proj : projectiles) {
        delete proj;
    }
    projectiles.clear();

    for (auto enemie : enemies) {
        delete enemie;
    }
    enemies.clear();
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
    
    quit = window->is_key_pressed(GLFW_KEY_ESCAPE) || window->window_should_close();
    player->update(*window, (float)delta_time);
    
    if (player->throw_projectile(*window)) {
        float angle = glm::radians(player->get_rotation());
        glm::vec2 scale = player->get_scale();
        glm::vec2 p_pos = player->get_position();
        glm::vec2 p_size = player->get_size();
        glm::vec2 pos = glm::vec2(
                                  p_pos.x - (p_size.x/2) - (p_size.x/2 * scale.x),
                                  p_pos.y - (p_size.y/2) - (p_size.y/2 * scale.y));
                                  
        glm::vec2 vel = glm::vec2(
                                  PROJECTILE_VELOCITY * glm::cos(angle),
                                  PROJECTILE_VELOCITY * glm::sin(angle));
        
        uint32_t cell_width = 16;
        uint32_t cell_height = 16;
        uint32_t rows = 1;
        uint32_t cols = 1;

        projectiles.push_back(new Projectile(
					     pos, vel,
					     glm::vec2(1.f, 1.f),
					     "projectile0",
					     cell_width, cell_height,
					     rows, cols));
    }

    for (uint32_t i = 0; i < enemies.size(); ++i) {
        auto enemie = enemies[i];

        // verify and resolve enemie x projectile collision
        for (uint32_t i = 0; i < projectiles.size(); ++i) {
            auto projectile = projectiles[i];
            glm::vec2 size0 = enemie->get_size() * enemie->get_scale();
            glm::vec2 size1 = projectile->get_size() * projectile->get_scale();
            glm::vec2 pos0 = enemie->get_position();
            glm::vec2 pos1 = projectile->get_position();
            
            if (collision_detect(pos0, size0, pos1, size1)) {
                projectile->ded = true;
                enemie->ded = true;
                std::cout << "kill enemie\n";
            }

            if (projectile->ded)
                projectiles.erase(projectiles.begin() + i);
        }
        
        
        if (enemie->ded)
            enemies.erase(enemies.begin() + i);
        else {
            enemie->update(*window, delta_time);

            // verify and resolve player x enemie collision
            glm::vec2 size0 = player->get_size() * player->get_scale();
            glm::vec2 size1 = enemie->get_size() * enemie->get_scale();
            glm::vec2 pos0 = player->get_position() - size0;
            glm::vec2 pos1 = enemie->get_position();
        
            if (collision_detect(pos0, size0, pos1, size1))
                std::cout << "collide player x enemie\n";
        }
    }

    for (auto projectile : projectiles) {
        projectile->update(*window, delta_time);   
    }

    sync_player_camera();
}

void Game::sync_player_camera() {

    std::cout << "x: " << player->get_position().x << " y: " << player->get_position().y << "\n";

    proj = glm::ortho(
		      0.f,//player->camera_offset.x,
		      (float)WIDTH,
		      0.f,//player->camera_offset.y,
		      (float)HEIGHT, -1.0f, 1.0f);

    view = glm::translate(glm::mat4(1.f), glm::vec3(0.f));
    player->camera = proj * view;
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
	tile_map->draw();	

	for (auto proj : projectiles) {
	    proj->draw();
        }

	for (auto enemie : enemies) {
            enemie->draw();
        }
    }
    Renderer::end_batch();                                                                
    Renderer::flush();        
    
    player->draw();
}

void Game::init() {
    window = new Window("Waves", WIDTH, HEIGHT);
    //window->set_custom_cursor_image("res/textures/mouse_icon.png");
    Renderer::init();

    load_textures();
    load_shaders();

    
    // load map from tiled editor project file
    tile_map = new Map("res/maps/map.tmj");
    
    // creating player with bat sprite sheet
    player = new Player(glm::vec2(WIDTH/2, HEIGHT/2), // world position
                        glm::vec2(200.f, 200.f), glm::vec2(1.f, 1.f),
                              "player", 32, 64, 8, 4); // tex_key, tile width, tile height, rows, cols

    // creating a weapon to player
    player->set_weapon(new Item(
				glm::vec2(0.f, 0.f), glm::vec2(0.f, 0.f),
				glm::vec2(1.f, 1.f), "weapon0", 64, 64, 1, 3));
    
    // view proj matrix
    translation = glm::vec3(WIDTH/2, HEIGHT/2, 0.f);
    proj = glm::ortho(0.0f, (float)WIDTH, 0.f, (float)HEIGHT, -1.0f, 1.0f);
    view = glm::translate(glm::mat4(1.0f), translation);

    player->camera_offset = translation;
    player->camera = proj * view;

    srand(time(0));
    for (uint32_t i = 0; i < 1; ++i) {
        glm::vec2 pos = glm::vec2(rand() % WIDTH, rand() % HEIGHT);                                  
        glm::vec2 vel = glm::vec2(ENEMIE_VELOCITY, ENEMIE_VELOCITY);
        
        uint32_t cell_width = 32;
        uint32_t cell_height = 32;
        uint32_t rows = 4;
        uint32_t cols = 4;
        enemies.push_back(new Enemie(
				     pos, vel,
				     glm::vec2(1.f, 1.f),
				     "enemie0",
				     cell_width, cell_height,
				     rows, cols));
        enemies[i]->set_target((Entity*)player);
    }
}

void Game::load_textures() {
    Renderer::load_texture("white", "");
    Renderer::load_texture("player", "res/textures/player.png");
    Renderer::load_texture("enemie0", "res/textures/32x32-bat-sprite.png");
    Renderer::load_texture("projectile0", "res/textures/clay2.png");
    Renderer::load_texture("weapon0", "res/textures/staff2.png");
}

void Game::load_shaders() {
    Renderer::load_shader("res/shaders/shader.vert", "res/shaders/shader.frag");
}

float Game::norma(glm::vec2 v0) {
    return (v0.x * v0.x + v0.y * v0.y);
}

float Game::sat_collision_detect(glm::vec2 pos0, glm::vec2 size0, glm::vec2 pos1, glm::vec2 size1) {
    glm::vec2 va[4], vb[4];
    va[0] = glm::vec2(pos0.x, pos0.y);
    va[1] = glm::vec2(pos0.x + size0.x, pos0.y);
    va[2] = glm::vec2(pos0.x + size0.x, pos0.y + size0.y);
    va[3] = glm::vec2(pos0.x, pos0.y + size0.y);

    vb[0] = glm::vec2(pos1.x, pos1.y);
    vb[1] = glm::vec2(pos1.x + size1.x, pos1.y);
    vb[2] = glm::vec2(pos1.x + size1.x, pos1.y + size1.y);
    vb[3] = glm::vec2(pos1.x, pos1.y + size1.y);

    
    float separation = -99999.0f;
    for (uint32_t i = 0; i < 4; ++i) {
        glm::vec2 axis = (va[(i + 1) % 4] - va[i]);
        glm::vec2 p_axis = glm::vec2(axis.y, -axis.x);
        glm::vec2 normal = glm::vec2(p_axis.x/norma(p_axis), p_axis.y/norma(p_axis));
        float minsep = 99999.0f;

        for (uint32_t j = 0; j < 4; ++j) {
            minsep = std::min(minsep, glm::dot(vb[j] - va[i], normal));   
        }
        
        if (minsep > separation) {
            separation = minsep;
        }
    }
    return separation;
}

bool Game::collision_detect(glm::vec2 pos0, glm::vec2 size0, glm::vec2 pos1, glm::vec2 size1) {
    return sat_collision_detect(pos0, size0, pos1, size1) <= 0 &&
        sat_collision_detect(pos1, size1, pos0, size0) <= 0;
}
