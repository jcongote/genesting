
#include <math.h>

#include "point.h"


float dot(point *a, point *b, point *c) //OK
{
    return ((b->x - a->x) * (c->x - b->x)) + ((b->y - a->y) * (c->y - b->y));
}

float cross(point *a, point *b, point *c) //OK
{
    return ((b->x - a->x)*(c->y - a->y)) - ((b->y - a->y)*(c->x - a->x));
}

float distance(point *a, point *b) //OK
{
    return sqrt(powf(a->x - b->x,2.0)+powf(a->y - b->y,2.0));
}
