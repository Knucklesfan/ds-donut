#pragma once
#include <nds.h>

#define BUFFER_WIDTH 32
#define BUFFER_HEIGHT 21
#define BUFFER_SIZE (BUFFER_WIDTH * BUFFER_HEIGHT)

#define IMAGE_SCALE 30
#define TWO_PI 6.28

// donut point traversal increment (inverse of density)
#define DELTA_J 0.07
#define DELTAJSIZE 90
#define DELTA_I 0.02
#define DELTAISIZE 314

#define R(mul, shift, x, y,underscore)            \
	underscore = x;                             \
	x -= mul * y >> shift;             \
	y += mul * underscore >> shift;             \
	underscore = 3145728 - x * x - y * y >> 11; \
	x = x * underscore >> 10;                   \
	y = y * underscore >> 10;

typedef struct
{
	int x;
	int y;

	u16* sprite_gfx_mem[32];
	int gfx_frame;


}sprite;
namespace Graphics {
    void bresenham(int8_t *buffer, int x1, int y1, int x2, int y2);

}
