#ifndef __CONSTS_H__
#define __CONSTS_H__

#define WIDTH 1200
#define HEIGHT 720
#define FPS 60
#define FRAME_TIME 1.0f/(float)FPS
#define MAX_QUAD_COUNT 1000
#define MAX_VERTEX_COUNT MAX_QUAD_COUNT * 4 // 4 vertex per quad
#define MAX_IDX_COUNT MAX_QUAD_COUNT * 6 // 6 indexes per quad
#define MAX_TEXTURES 32
#define TILE_WIDTH 32
#define TILE_HEIGHT 32
#define MAP_RELATIVE_PATH "res/maps/"
#define TILESET_ROWS 10
#define TILESET_COLS 10
#define TILESET_NAME "tilemap"
#define DEFAULT_ANIMATION_TIME 0.1
#define PLAYER_ANIMATION_TIME 0.1
#define PLAYER_MAX_SHOOT_TIME 0.3
#define PLAYER_LIFEBAR_WIDTH 70
#define PLAYER_LIFEBAR_HEIGHT 10
#define PROJECTILE_VELOCITY 400
#define PROJECTILE_ANIMATION_TIME 0.1
#define PROJECTILE_MAX_LIFE_TIME 3
#define ENEMIE_ANIMATION_TIME 0.1
#define ENEMIE_VELOCITY 100
#define ITEM_ANIMATION_TIME 0.1
#define PLAYER_LIFE_POINTS 1000

#endif // __CONSTS_H__: header
