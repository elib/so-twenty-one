#include "math.h"

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