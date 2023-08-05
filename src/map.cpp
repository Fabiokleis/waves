#include "map.hpp"
#include <tileson.hpp>
#include <filesystem>
#include "consts.hpp"
#include "renderer.hpp"
#include "animation.hpp"
		
Map::Map(const std::string& map_file_path) {
    tson::Tileson t;
    tiled_map = t.parse(std::filesystem::path(map_file_path));
    if (tiled_map->getStatus() == tson::ParseStatus::OK) {

	width = tiled_map->getSize().x;
	height = tiled_map->getSize().y;

	uint32_t cell_width = tiled_map->getTileset(TILESET_NAME)->getTileSize().x;
	uint32_t cell_height = tiled_map->getTileset(TILESET_NAME)->getTileSize().y;

	cols = (uint32_t) tiled_map->getTileset(TILESET_NAME)->getColumns();

	std::string tileset_path = MAP_RELATIVE_PATH; 
	Renderer::load_texture("tilemap", tileset_path.append(tiled_map->getTileset(TILESET_NAME)->getImagePath().c_str()));

	tson::Layer *tile_layer = tiled_map->getLayer("Tile Layer 1");
	sprite_sheet = new SpriteSheet("tilemap", cols, cols, cell_width, cell_height);
	
        data = tile_layer->getData();

	populate_tiles();
    } else {
	std::cerr << "Could not load tiled editor map!\n";
	exit(1);
    }
}

Map::~Map() {
    delete tiled_map.release();
    tiles.clear();
}

void Map::populate_tiles() {

    for (uint32_t i = 0; i < data.size(); ++i) {
      	
	Tile t = (Tile){
	    .id = data[i] - 1,
	    .row = (data[i] - 1) / cols,
	    .col = (data[i] - 1) % cols,
	    .size = glm::vec2(TILE_WIDTH, TILE_HEIGHT),
	};
	tiles.push_back(t);
    }

    uint32_t stride = 0;
    for (uint32_t y = 0; y < height; ++y) {
	for (uint32_t x = 0; x < width; ++x) {
	    tiles[x+stride].pos = glm::vec2(x * TILE_WIDTH, y * TILE_HEIGHT);
	}
	stride += width;
    }
}

void Map::draw() {

    for (auto t : tiles) {
	glm::vec4 uv = Animation::calculate_sprite_position(*sprite_sheet, t.row, t.col);
	Renderer::draw_quad(t.pos, t.size, uv, sprite_sheet->tex); 
    }
}
