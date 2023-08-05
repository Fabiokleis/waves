#ifndef __MAP_H__
#define __MAP_H__

#include <tileson.hpp>
#include <vector>

#include "consts.hpp"
#include "entity.hpp"
#include "spritesheet.hpp"

class Map {
private:

    struct Tile {
	uint32_t id;
	uint32_t row;
	uint32_t col;
	glm::vec2 pos;
	glm::vec2 size;
    };

    uint32_t cols;
    uint32_t width;
    uint32_t height;
    
    std::unique_ptr<tson::Map> tiled_map;
    std::vector<uint32_t> data;
    std::vector<Tile> tiles;
    SpriteSheet *sprite_sheet = nullptr;

    void populate_tiles();
    
public:
    Map(const std::string& map_file_path);
    ~Map();

    void update() {}
    void draw();
};

#endif // __MAP_H__: header
