#include "entity.hpp"

Entity::Entity(glm::vec2 pos, glm::vec2 size, glm::vec2 vel, glm::vec2 scale, glm::vec4 color) {
    this->body.position = pos;
    this->body.size = size;
    this->body.velocity = vel;
    this->body.scale = scale;
    this->body.color = color;
}
