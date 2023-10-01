#include "spritesheet.hpp"

SpriteSheet::SpriteSheet(
			 const std::string& tex_key,
			 uint32_t rows, uint32_t cols,
			 uint32_t cell_width, uint32_t cell_height) :
    tex(tex_key), rows(rows), cols(cols),
    cell_width(cell_width), cell_height(cell_height)
{} 

SpriteSheet::~SpriteSheet() {}
