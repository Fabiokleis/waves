#ifndef __ENTITY_H__
#define __ENTITY_H__

#include <glm/glm.hpp>
#include <stdint.h>
#include "spritesheet.hpp"

class Entity {
private:
    SpriteSheet *sheet = nullptr;
    struct Body {
        glm::vec2 position;
        glm::vec2 velocity;
        glm::vec2 size;
        glm::vec4 color;
    };

protected:
    Body body;    
    void add_sprite_sheet(uint32_t texture_idx, uint32_t cell_width, uint32_t cell_height, uint32_t rows, uint32_t cols);

    
public:
    Entity(glm::vec2 pos, glm::vec2 size, glm::vec2 vel, glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f });    
    ~Entity()
    {
        if (this->sheet)
            delete this->sheet;
    }

    inline const glm::vec2& get_position() const { return this->body.position; }
    inline const glm::vec2& get_size() const { return this->body.size; }
    inline const glm::vec4& get_color() const { return this->body.color; }
    inline const SpriteSheet& get_sprite_sheet() const { return *this->sheet; }
    void set_position(glm::vec2 pos) { this->body.position = pos; }

};

#endif // __ENTITY_H__: header
