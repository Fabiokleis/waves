#ifndef __PROJECTILE_H__
#define __PROJECTILE_H__

#include <glm/glm.hpp>
#include "entity.hpp"
#include "spritesheet.hpp"
#include "animation.hpp"
#include "window.hpp"

class Projectile : public Entity {
private:
    Animation *animation = nullptr;
    SpriteSheet *sprite_sheet = nullptr;
    float life_time = 0.f;
 
    
public:
    Projectile(glm::vec2 pos, glm::vec2 size, glm::vec2 vel, glm::vec2 scale);
    Projectile(
               glm::vec2 pos,
               glm::vec2 vel,
               glm::vec2 scale,
               const std::string &texture_key,
               uint32_t cell_width,
               uint32_t cell_height,
               uint32_t rows,
               uint32_t cols);
    ~Projectile();

    bool ded = false;
    void update(const Window &window, float dt) override;
    void draw() override;
private:
    
};

#endif // __PROJECTILE_H__: header
