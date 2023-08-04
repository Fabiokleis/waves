#include "player.hpp"
#include "consts.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "renderer.hpp"

#include <iostream>

Player::Player(
           glm::vec2 pos, glm::vec2 vel, glm::vec2 scale,
           uint32_t texture_idx,
           uint32_t cell_width, uint32_t cell_height,
           uint32_t rows, uint32_t cols) :
    Entity(pos, glm::vec2(cell_width, cell_height), vel, scale) 
{
    animation = new Animation(PLAYER_ANIMATION_TIME);
    sprite_sheet = new SpriteSheet(texture_idx, rows, cols, cell_width, cell_height);
    lifebar = new StatusBar(
                            this,
                            glm::vec2(PLAYER_LIFEBAR_WIDTH, PLAYER_LIFEBAR_HEIGHT),
                            glm::vec4(0.0, 1.f, 0.f, 1.f),
                            glm::vec4(1.f, 0.f, 0.f, 1.f));
}

void Player::draw() {    
    glm::vec4 color = this->body.color;
    // each player draw call will do a OpenGL draw call
    Renderer::begin_batch();
    Renderer::get_shader(0)->Bind();
    Renderer::get_shader(0)->SetUniform4f("u_color", color.x, color.y, color.z, color.w);

    Renderer::get_shader(0)->SetUniformMat4f("u_model", this->model);
   
    Renderer::get_shader(0)->SetUniformMat4f("u_camera", this->camera);

    glm::vec4 uv = this->animation->get_uv(*this->sprite_sheet, this->flip, 0);

    // to apply rotation around Z axis at center of the body 
    glm::vec2 pos = glm::vec2(-this->body.size.x/2, -this->body.size.y/2);
    
    Renderer::draw_quad(
                        pos, // local position
                        this->body.size, uv,
                        this->sprite_sheet->tex_idx);

    Renderer::end_batch(); // copy cpu buffer to gpu
    Renderer::flush(); // draw triangles


    // life bar and weapon draw
    Renderer::begin_batch();
    Renderer::get_shader(0)->Bind();
    Renderer::get_shader(0)->SetUniform4f("u_color", 1.f, 1.f, 1.f, 1.f);
    Renderer::get_shader(0)->SetUniformMat4f("u_model", weapon->get_model());
    Renderer::get_shader(0)->SetUniformMat4f("u_camera", this->camera);
    weapon->draw();
 
    Renderer::end_batch();
    Renderer::flush();

    Renderer::begin_batch();
    Renderer::get_shader(0)->Bind();
    Renderer::get_shader(0)->SetUniform4f("u_color", 1.f, 1.f, 1.f, 1.f);
    Renderer::get_shader(0)->SetUniformMat4f("u_model", glm::mat4(1.f));
    Renderer::get_shader(0)->SetUniformMat4f("u_camera", this->camera);

    lifebar->draw();
    Renderer::end_batch();
    Renderer::flush();
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


    // weapon model
    glm::mat4 w_rot = glm::rotate(glm::mat4(1.0f), glm::radians(looking_at - 45.f), axis);
    glm::mat4 w_trans = glm::translate(glm::mat4(1.0f), glm::vec3(5.f, -20.f, 0.f) + pos); // align with player position
    glm::mat4 w_scal = glm::scale(glm::mat4(1.f), glm::vec3(weapon->get_scale().x, weapon->get_scale().y, 1.f));
    glm::mat4 w_model = w_trans * w_rot * w_scal;
    weapon->set_model(w_model);
}

void Player::look_at_front_of_mouse(glm::vec2 mouse_pos, glm::vec2 window_size) {
    float y = window_size.y - mouse_pos.y;
    glm::vec2 mapped_mouse = glm::vec2(mouse_pos.x, y);

    float scaler = body.scale.x;
    
    float ca = mapped_mouse.x - (this->body.position.x  - (this->body.size.x/2 * scaler));
    float co =  mapped_mouse.y - (this->body.position.y  - (this->body.size.y/2 * scaler));

    float rotation = glm::degrees(glm::atan(co, ca));

    this->looking_at = rotation;

    if (this->looking_at < 90 && this->looking_at > 0)
        this->flip = true;
    else if (this->looking_at > -90 && this->looking_at < 0)
        this->flip = true;
    else
        this->flip = false;

    //weapon->flip = !flip;
    this->apply_player_model(scaler, glm::vec3(0, 0, 1), 0.0);
}

void Player::move(glm::vec2 dir, float delta_time) {
    glm::vec2 vel = this->body.velocity * dir;
    this->body.position += vel * delta_time;
}

bool Player::throw_projectile(const Window& window) {

    if (shoot_timer > PLAYER_MAX_SHOOT_TIME) {
        shoot_timer = 0;
    }
    return !shoot_timer && window.is_mouse_button_pressed(GLFW_MOUSE_BUTTON_LEFT);
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
    
    this->shoot_timer += delta_time;
    
    this->look_at_front_of_mouse(window.get_mouse_pos(), window.get_size());
    this->animation->update(*this->sprite_sheet, delta_time);


    // begin life bar at bottom left bound of player
    lifebar->set_position(glm::vec2(
                                    body.position.x - (PLAYER_LIFEBAR_WIDTH/2 + body.size.x/2),
                                    body.position.y - (PLAYER_LIFEBAR_HEIGHT + (body.size.y * body.scale.y) + 10)));
    
    weapon->update(window, delta_time);
    //lifebar->update(window, delta_time);
}
