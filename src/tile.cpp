#include "tile.hpp"
#include "consts.hpp"
#include "renderer.hpp"


Tile::Tile(
           glm::vec2 pos,
           glm::vec2 vel,
           glm::vec2 scale,
           uint32_t texture_idx,
           uint32_t cell_width,
           uint32_t cell_height,
           uint32_t rows,
           uint32_t cols) :
    Entity(pos, glm::vec2(cell_width, cell_height), vel, scale)
{
    animation = new Animation(DEFAULT_ANIMATION_TIME);
    sprite_sheet = new SpriteSheet(texture_idx, rows, cols, cell_width, cell_height);
}

void Tile::update(const Window& window, float delta_time) {
    animation->update(*sprite_sheet, delta_time);
}

void Tile::draw() {
    glm::vec4 uv = animation->get_uv(*sprite_sheet, false, 0);
    Renderer::draw_quad(body.position, body.size, uv, sprite_sheet->tex_idx);
}
