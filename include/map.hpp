#ifndef __MAP_H__
#define __MAP_H__

#include "entity.hpp"
#include "animation.hpp"
#include "spritesheet.hpp"
#include "window.hpp"
#include <vector>

#define MAP_CELLS 32

class Tile : public Entity {
private:
    
public:
    Tile(
          glm::vec2 pos,
          glm::vec2 vel,
          glm::vec2 scale,
          uint32_t texture_idx,
          uint32_t cell_width,
          uint32_t cell_height,
          uint32_t rows,
          uint32_t cols
          );
    ~Tile() {
        if (nullptr != animation)
            delete animation;
        if (nullptr != sprite_sheet)
            delete sprite_sheet;
    }

    Animation *animation = nullptr;
    SpriteSheet *sprite_sheet = nullptr;

    void draw() override;
    void update(const Window& window, float delta_time) override;
};


class Map {
        
private:
    std::vector<std::vector<Tile *>> tiles;
    
public:
    Map();
    ~Map();
};

#endif // __MAP_H__: header
