#ifndef __SPRITE_SHEET_H__
#define __SPRITE_SHEET_H__
#include <stdint.h>

class SpriteSheet {
public:
    uint32_t rows;
    uint32_t cols;
    float cell_width;
    float cell_height;

    uint32_t tex_id;

    SpriteSheet(uint32_t tex_id, uint32_t rows, uint32_t cols, uint32_t cell_width, uint32_t cell_height);
    ~SpriteSheet();
};



#endif //__SPRITE_SHEET_H__: header
