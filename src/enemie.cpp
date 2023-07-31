#include "enemie.hpp"
#include "consts.hpp"
#include "window.hpp"
#include <glm/glm.hpp>
#include "renderer.hpp"

Enemie::Enemie(
               glm::vec2 pos, glm::vec2 vel, glm::vec2 scale,
               uint32_t texture_idx,
               uint32_t cell_width, uint32_t cell_height,
               uint32_t rows, uint32_t cols) :
    Entity(pos, glm::vec2(cell_width, cell_height), vel, scale)
{
    animation = new Animation(ENEMIE_ANIMATION_TIME);
    sprite_sheet = new SpriteSheet(texture_idx, rows, cols, cell_width, cell_height);
}

void Enemie::draw() {
    glm::vec4 uv = animation->get_uv(*sprite_sheet, true, 0);
    Renderer::draw_quad(body.position, body.size, uv, sprite_sheet->tex_idx);
}

void Enemie::update(const Window& window, float delta_time) {

    if (nullptr == target)
        return;

    glm::vec2 t_scale = target->get_scale();
    glm::vec2 t_size = target->get_size();
    glm::vec2 t_pos = target->get_position();
    glm::vec2 dir = glm::vec2(0.f);

    glm::vec2 t_origin = glm::vec2(t_size.x * t_scale.x, t_size.y * t_scale.y);
    t_pos -= t_origin;

    if (t_pos.x > body.position.x)
        dir.x = 1.f;
    else
        dir.x = -1.f;

    if (t_pos.y > body.position.y)
        dir.y = 1.f;
    else
        dir.y = -1.f;

    glm::vec2 vel = body.velocity * dir;
    body.position += vel * delta_time;

    animation->update(*sprite_sheet, delta_time);
}
