#ifndef __ENEMIE_H__
#define __ENEMIE_H__
#include <glm/glm.hpp>
#include "entity.hpp"
#include "animation.hpp"
#include "spritesheet.hpp"

class Enemie : public Entity {
private:
    Animation *animation = nullptr;
    SpriteSheet *sprite_sheet = nullptr;
    Entity *target = nullptr;

public:
    Enemie(
           glm::vec2 pos, glm::vec2 vel, glm::vec2 scale,
           const std::string& texture_key,
           uint32_t cell_width, uint32_t cell_height,
           uint32_t rows, uint32_t cols);
    ~Enemie() {
        if (animation)
            delete animation;
        if (sprite_sheet)
            delete sprite_sheet;
    }

    bool ded = false;
    void set_target(Entity *follow) {
        if (target == nullptr)
            target = follow;
    } 
    void draw() override;
    void update(const Window& window, float delta_time) override;
};

#endif // __ENEMIE_H__: header
