#include "item.hpp"
#include "consts.hpp"
#include "renderer.hpp"

Item::Item(
           glm::vec2 pos,
           glm::vec2 vel,
           glm::vec2 scale,
           uint32_t texture_idx,
           uint32_t cell_width,
           uint32_t cell_height,
           uint32_t rows,
           uint32_t cols
           ) :
    Entity(pos, glm::vec2(cell_width, cell_height), vel, scale)
{
    animation = new Animation(ITEM_ANIMATION_TIME);
    sprite_sheet = new SpriteSheet(texture_idx, rows, cols, cell_width, cell_height);
}

Item::~Item() {
    if (nullptr != animation)
        delete animation;
    if (nullptr != sprite_sheet)
        delete sprite_sheet;
}

void Item::update(const Window &window, float delta_time) {
    animation->update(*sprite_sheet, delta_time);
}

void Item::draw() {
    glm::vec4 uv = animation->get_uv(*sprite_sheet, flip, 0);
    Renderer::draw_quad(body.position, body.size, uv, sprite_sheet->tex_idx);
}
