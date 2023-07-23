#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "entity.hpp"
#include "animation.hpp"
#include "spritesheet.hpp"
#include "window.hpp"

class Player : public Entity {
private:
    Animation *animation = nullptr;
    SpriteSheet *sprite_sheet = nullptr;
public:
    Player(
           glm::vec2 pos, glm::vec2 vel,
           uint32_t texture_idx,
           uint32_t cell_width, uint32_t cell_height,
           uint32_t rows, uint32_t cols);

    ~Player() {
        delete animation;
        delete sprite_sheet;
    }

    void draw();
    void update(const Window& window, float delta_time);
    void move(glm::vec2 dir, float delta_time);
};


#endif // __PLAYER_H__: header
