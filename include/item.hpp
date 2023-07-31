#ifndef __ITEM_H__
#define __ITEM_H__

#include "entity.hpp"
#include "animation.hpp"
#include "spritesheet.hpp"
#include "window.hpp"

class Item : public Entity {
private:
    Animation *animation = nullptr;
    SpriteSheet *sprite_sheet = nullptr;
    glm::mat4 model = glm::mat4(1.f);
    
public:
    Item(
         glm::vec2 pos,
         glm::vec2 vel,
         glm::vec2 scale,
         uint32_t texture_idx,
         uint32_t cell_width,
         uint32_t cell_height,
         uint32_t rows,
         uint32_t cols
         );
    ~Item();

    bool flip = false;

    void set_model(glm::mat4 n_model) { model = n_model; }
    glm::mat4 get_model() const { return model; }
    void update(const Window &window, float delta_time) override;
    void draw() override;
};

#endif // __ITEM_H__: header
