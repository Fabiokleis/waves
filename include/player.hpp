#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "gamedata.hpp"
#include "entity.hpp"
#include <stdint.h>

class Player : public Entity {
    
public:
    Player(
           glm::vec2 pos, glm::vec2 vel,
           uint32_t texture_idx,
           uint32_t cell_width, uint32_t cell_height,
           uint32_t rows, uint32_t cols);

    ~Player(){}

    void draw(GameData &gd);
    void update();
};


#endif // __PLAYER_H__: header
