#ifndef FILE_POINT_H
#define FILE_POINT_H

struct _point
{
    float   x,y;
};

typedef struct _point point;

float dot(point *a, point *b, point *c);
float cross(point *a, point *b, point *c);
float distance(point *a, point *b);

#endif
