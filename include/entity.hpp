#ifndef __ENTITY_H__
#define __ENTITY_H__

#include <glm/glm.hpp>
#include "window.hpp"

class Entity {
private:
    struct Body {
        glm::vec2 position;
        glm::vec2 velocity;
        glm::vec2 size;
        glm::vec4 color;
    };

protected:
    Body body;    
    
public:
    Entity(glm::vec2 pos, glm::vec2 size, glm::vec2 vel, glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f });    
    ~Entity() {}

    inline const glm::vec2& get_position() const { return this->body.position; }
    inline const glm::vec2& get_size() const { return this->body.size; }
    inline const glm::vec4& get_color() const { return this->body.color; }
    void set_position(glm::vec2 pos) { this->body.position = pos; }
    void set_velocity(glm::vec2 vel) { this->body.velocity = vel; }

    virtual void update(const Window &window, float delta_time) = 0;
    virtual void draw() = 0;

};

#endif // __ENTITY_H__: header
