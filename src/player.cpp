#include "player.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "renderer.hpp"

#define ANIMATION_TIME 0.1

Player::Player(
           glm::vec2 pos, glm::vec2 vel,
           uint32_t texture_idx,
           uint32_t cell_width, uint32_t cell_height,
           uint32_t rows, uint32_t cols) :
        Entity(pos, glm::vec2(cell_width, cell_height), vel) 
{
    this->animation = new Animation(ANIMATION_TIME);;
    this->sprite_sheet = new SpriteSheet(texture_idx, rows, cols, cell_width, cell_height);
}

void Player::draw() {
    glm::vec4 color = this->body.color;
    // each player draw call will do a OpenGL draw call
    Renderer::begin_batch();
    Renderer::get_shader(0)->Bind();
    Renderer::get_shader(0)->SetUniform4f("u_color", color.x, color.y, color.z, color.w);
    
    glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(3, 3, 0.0));
    glm::mat4 mvp = Renderer::get_camera() * model;
    Renderer::get_shader(0)->SetUniformMat4f("u_MVP", mvp);

    glm::vec4 uv = this->animation->get_uv(*this->sprite_sheet, true, 1);

    Renderer::draw_quad(
                 this->body.position,
                 this->body.size, uv,
                 this->sprite_sheet->tex_idx);

    Renderer::end_batch(); // copy cpu buffer to gpu
    Renderer::flush(); // draw triangles
}

void Player::move(glm::vec2 dir, float delta_time) {
    glm::vec2 vel = this->body.velocity * dir;
    this->body.position += vel * delta_time;
}

void Player::update(const Window &window, float delta_time) {

    if (window.is_key_pressed(GLFW_KEY_LEFT) || window.is_key_pressed(GLFW_KEY_A)) {
        this->move(glm::vec2(-1.0f, 0.0f), delta_time);
    }

    if (window.is_key_pressed(GLFW_KEY_RIGHT) || window.is_key_pressed(GLFW_KEY_D)) {
        this->move(glm::vec2(1.0f, 0.0f), delta_time);
    }

    if (window.is_key_pressed(GLFW_KEY_UP) || window.is_key_pressed(GLFW_KEY_W)) {
        this->move(glm::vec2(0.0f, 1.0f), delta_time);
    }

    if (window.is_key_pressed(GLFW_KEY_DOWN) || window.is_key_pressed(GLFW_KEY_S)) {
        this->move(glm::vec2(0.0f, -1.0f), delta_time);
    }

    this->animation->update(*this->sprite_sheet, delta_time);
}
