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

#endif // __CONSTS_H__: header
