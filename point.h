#ifndef FILE_POINT_H
#define FILE_POINT_H

struct _point
{
    float   x,y;
};

typedef struct _point point;

float point_dot(point *a, point *b, point *c);

float point_cross(point *a, point *b, point *c);

#endif
