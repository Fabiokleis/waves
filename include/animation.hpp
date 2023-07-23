#ifndef __ANIMATION_H__
#define __ANIMATION_H__
#include <glm/glm.hpp>

#include "spritesheet.hpp"

class Animation
{
public:
    float total_time;
    float switch_time;

    uint32_t count_cols;

    Animation(float switch_time);
    ~Animation();

    glm::vec4 get_uv(const SpriteSheet &sheet, bool flip, uint32_t row);
    glm::vec4 calculate_sprite_position(const SpriteSheet &sheet, uint32_t row, uint32_t col);
    void update(const SpriteSheet &sheet, float dt);
};

#endif // __ANIMATION_H__: header
