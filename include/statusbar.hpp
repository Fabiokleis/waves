#ifndef __STATUSBAR_H__
#define __STATUSBAR_H__

#include "entity.hpp"
#include "window.hpp"
#include <glm/glm.hpp>

class StatusBar : public Entity {
private:
    glm::vec2 fg_size;
    glm::vec4 fg_color = glm::vec4(0.f);

    Entity *owner = nullptr;
    
public:
    StatusBar(Entity *n_owner, glm::vec2 size, glm::vec4 bg_color, glm::vec4 fr_color);
    ~StatusBar();

    void set_color(glm::vec4 bg_color, glm::vec4 fg_color) {
        this->fg_color = fg_color;
        this->body.color = bg_color;
    }
  
    void decrease_width(float w) {
        this->body.size = glm::vec2(w, this->body.size.y);
    }
  
    void draw() override;
    void update(const Window& window, float delta_time) override;
};

#endif // __STATUSBAR_H__: header
