#include "point.h"

float point_dot(point *a, point *b, point *c) //OK
{
    return ((b->x - a->x) * (c->x - b->x)) + ((b->y - a->y) * (c->y - b->y));
}

float point_cross(point *a, point *b, point *c) //OK
{
    return ((b->x - a->x)*(c->y - a->y)) - ((b->y - a->y)*(c->x - a->x));
}
