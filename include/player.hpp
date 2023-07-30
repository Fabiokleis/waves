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

    glm::mat4 model = glm::mat4(1.f); // Mmodel . Vlocal
    float scale_factor = 1.f;
    float looking_at = 0.f;
    float shoot_timer = 0.f;
    
    void move(glm::vec2 dir, float delta_time);
    void look_at_front_of_mouse(glm::vec2 mouse_pos, glm::vec2 window_size);
    void apply_player_model(float scaler, glm::vec3 axis, float rotation);
    
    
public:
    Player(
           glm::vec2 pos, glm::vec2 vel,
           uint32_t texture_idx,
           uint32_t cell_width, uint32_t cell_height,
           uint32_t rows, uint32_t cols);

    ~Player() {
        if (animation)
            delete animation;
        if (sprite_sheet)
            delete sprite_sheet;
    }

    glm::mat4 camera = glm::mat4(1.f);
    glm::vec3 camera_offset = glm::vec3(0.f);

    float get_rotation() const { return looking_at; }
    float get_scale() const { return scale_factor; }

    bool throw_projectile(const Window& window);
    
    void draw() override;
    void update(const Window& window, float delta_time) override;

};


#endif // __PLAYER_H__: header
