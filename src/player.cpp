#include "player.hpp"
#include "consts.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "renderer.hpp"

#include <iostream>

Player::Player(
           glm::vec2 pos, glm::vec2 vel,
           uint32_t texture_idx,
           uint32_t cell_width, uint32_t cell_height,
           uint32_t rows, uint32_t cols) :
        Entity(pos, glm::vec2(cell_width, cell_height), vel) 
{
    this->animation = new Animation(PLAYER_ANIMATION_TIME);
    this->sprite_sheet = new SpriteSheet(texture_idx, rows, cols, cell_width, cell_height);
    this->scale_factor = 2.f;
}

void Player::draw() {
    glm::vec4 color = this->body.color;
    // each player draw call will do a OpenGL draw call
    Renderer::begin_batch();
    Renderer::get_shader(0)->Bind();
    Renderer::get_shader(0)->SetUniform4f("u_color", color.x, color.y, color.z, color.w);

    Renderer::get_shader(0)->SetUniformMat4f("u_model", this->model);
   
    Renderer::get_shader(0)->SetUniformMat4f("u_camera", this->camera);

    glm::vec4 uv = this->animation->get_uv(*this->sprite_sheet, true, 0);

    // to apply rotation around Z axis at center of the body 
    glm::vec2 pos = glm::vec2(-this->body.size.x/2, -this->body.size.y/2);
    
    Renderer::draw_quad(
                        pos, // local position
                        this->body.size, uv,
                        this->sprite_sheet->tex_idx);

    Renderer::end_batch(); // copy cpu buffer to gpu
    Renderer::flush(); // draw triangles
}

void Player::apply_player_model(float scaler, glm::vec3 axis, float rotation) {
    glm::vec3 scalar_vec = glm::vec3(scaler, scaler, 1.0);
    
    glm::vec3 pos = glm::vec3(
                              this->body.position.x - (this->body.size.x/2 * scaler),
                              this->body.position.y - (this->body.size.y/2 * scaler), 0.0f);

    
    glm::mat4 translation = glm::translate(glm::mat4(1.0f), pos); // set object position in world
    glm::mat4 scal = glm::scale(glm::mat4(1.0f), scalar_vec); // scale player size by scale factor
    glm::mat4 rot = glm::rotate(glm::mat4(1.0f), glm::radians(rotation), axis); // apply rotation

    this->model = translation * rot * scal; // player model matrix
}

void Player::look_at_front_of_mouse(glm::vec2 mouse_pos, glm::vec2 window_size) {
    float y = window_size.y - mouse_pos.y;
    glm::vec2 mapped_mouse = glm::vec2(mouse_pos.x, y);

    float scaler = this->scale_factor;
    
    float ca = mapped_mouse.x - (this->body.position.x  - (this->body.size.x/2 * scaler));
    float co =  mapped_mouse.y - (this->body.position.y  - (this->body.size.y/2 * scaler));

    // 90 degress just to fit sprite position
    float rotation = 90 + glm::degrees(glm::atan(co, ca));

    this->looking_at = rotation;

    this->apply_player_model(scaler, glm::vec3(0, 0, 1), rotation);
}

void Player::move(glm::vec2 dir, float delta_time) {
    glm::vec2 vel = this->body.velocity * dir;
    this->body.position += vel * delta_time;
}

bool Player::throw_projectile(const Window& window) {
    return shoot_timer >= PLAYER_MAX_SHOOT_TIME &&
        window.is_mouse_button_pressed(GLFW_MOUSE_BUTTON_LEFT);
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

    if (shoot_timer >= PLAYER_MAX_SHOOT_TIME)
        shoot_timer = 0;
    else
        this->shoot_timer += delta_time;
    
    this->look_at_front_of_mouse(window.get_mouse_pos(), window.get_size());
    this->animation->update(*this->sprite_sheet, delta_time);
}
