#include "entity.hpp"

Entity::Entity(glm::vec2 pos, glm::vec2 size, glm::vec2 vel, glm::vec4 color) {
    this->body.position = pos;
    this->body.size = size;
    this->body.velocity = vel;
    this->body.color = color;
}

void Entity::add_sprite_sheet(uint32_t texture_idx, uint32_t cell_width, uint32_t cell_height, uint32_t rows, uint32_t cols) {
    if (this->sheet == nullptr)
        this->sheet = new SpriteSheet(texture_idx, rows, cols, cell_width, cell_height);
}
