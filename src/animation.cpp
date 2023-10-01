#include <glm/glm.hpp>
#include "animation.hpp"
#include "renderer.hpp"

Animation::Animation(float switch_time) :
    total_time(0), switch_time(switch_time), count_cols(0)
{
}

Animation::~Animation() {}

void Animation::update(const SpriteSheet &sheet, float dt)
{
    this->total_time += dt;

    if (this->total_time > this->switch_time) {
        this->total_time = 0.0f;
        this->count_cols++;

        if (this->count_cols == sheet.cols)
            this->count_cols = 0;
    }
}

glm::vec4 Animation::get_uv(const SpriteSheet &sheet, bool flip, uint32_t row) {
    glm::vec4 uv = Animation::calculate_sprite_position(sheet, row, this->count_cols);
    if (flip) {
        float x = uv.x;
        uv.x = uv.z;
        uv.z = x;
        return uv;
    }
    return uv;
}

glm::vec4 Animation::calculate_sprite_position(const SpriteSheet &sheet, uint32_t row, uint32_t col) {
    glm::vec2 tex_size = Renderer::get_texture_size(sheet.tex);
    float w = 1.0f / (tex_size.x / sheet.cell_width);
    float h = 1.0f / (tex_size.y / sheet.cell_height);
    float x = col * w;
    
    float y = ((sheet.rows - 1) - row) * h;

    return glm::vec4(x, y, x + w, y + h);
}
