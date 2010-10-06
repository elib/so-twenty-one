#include "math.h"
#include <allegro5/allegro.h>


#pragma once


inline int round(double value)
{
	return (int)floor(value+0.5);
}


#define MAX(a,b) ((a > b) ? a : b)
#define MIN(a,b) ((a < b) ? a : b)

typedef struct DOUBLE_RECT
{
	double x, y, width, height;
} Rect;


// x - sin(x*2*PI) / (2*PI)
inline double ease_in(double t) { return t - (sin(pow(t, 2) * 2 * ALLEGRO_PI) / (2* ALLEGRO_PI)); }
inline double ease_out(double t) { return 1.0 - t + (sin(pow(t, 2) * 2 * ALLEGRO_PI) / (2* ALLEGRO_PI)); }