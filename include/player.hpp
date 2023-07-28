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

    glm::mat4 model; // Mmodel . Vlocal

    float scale_factor;
    
    void move(glm::vec2 dir, float delta_time);
    void look_at_front_of_mouse(glm::vec2 mouse_pos, glm::vec2 window_size);
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
    void apply_player_model(float scaler, glm::vec3 axis, float rotation);

};


#endif // __PLAYER_H__: header
