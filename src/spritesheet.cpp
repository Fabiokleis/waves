#include "spritesheet.hpp"


SpriteSheet::SpriteSheet(uint32_t tex_id, uint32_t rows, uint32_t cols, uint32_t cell_width, uint32_t cell_height) :
    tex_id(tex_id), rows(rows), cols(cols), cell_width(cell_width), cell_height(cell_height)
{} 

SpriteSheet::~SpriteSheet() {}
