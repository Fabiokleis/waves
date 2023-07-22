#include "player.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Player::Player(
           glm::vec2 pos, glm::vec2 vel,
           uint32_t texture_idx,
           uint32_t cell_width, uint32_t cell_height,
           uint32_t rows, uint32_t cols) :
        Entity(pos, glm::vec2(cell_width, cell_height), vel) 
{
    this->add_sprite_sheet(texture_idx, cell_width, cell_height, rows, cols);
}

void Player::draw(GameData &gd) {
    // each player draw call will do a OpenGL draw call
    gd.begin_batch();

    
    gd.shader_map[0]->Bind();
    gd.shader_map[0]->SetUniform4f("u_color", 1, 1, 1, 1.0f);                        
    glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(2, 2, 0.0));
    glm::mat4 mvp = gd.proj * gd.view * model;
    gd.shader_map[0]->SetUniformMat4f("u_MVP", mvp);

    gd.draw_quad(this->get_position(), this->get_sprite_sheet(), 0, 3, false);

    gd.end_batch(); // copy cpu buffer to gpu
    gd.flush(); // draw triangles
}

void Player::update() {}
