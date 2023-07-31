#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "projectile.hpp"
#include "renderer.hpp"
#include "consts.hpp"

Projectile::Projectile(glm::vec2 pos, glm::vec2 size, glm::vec2 vel, glm::vec2 scale) :
    Entity(pos, size, vel, scale)
{
}

Projectile::Projectile(
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
    this->animation = new Animation(PROJECTILE_ANIMATION_TIME);
    this->sprite_sheet = new SpriteSheet(texture_idx, rows, cols, cell_width, cell_height);
    this->life_time = 0.f;
}
    
Projectile::~Projectile() {
    if (nullptr != this->animation)
        delete this->animation;
    if (nullptr != this->sprite_sheet)
        delete this->sprite_sheet;
}

void Projectile::update(const Window &window, float dt) {
    life_time += dt;
    if (life_time >= PROJECTILE_MAX_LIFE_TIME)
        ded = true;
    
    glm::vec2 vel = this->body.velocity * dt;
    this->body.position += vel;
}

void Projectile::draw() {
    glm::vec4 uv = this->animation->get_uv(*this->sprite_sheet, true, 0);
    Renderer::draw_quad(body.position, body.size, uv, sprite_sheet->tex_idx);
}
