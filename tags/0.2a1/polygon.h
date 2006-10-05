#ifndef FILE_POLYGON_H
#define FILE_POLYGON_H

#include <stdbool.h>

#include "point.h"
#include "line.h"


struct _polygon
{
    unsigned int nvertices;
    point *v;
};

typedef struct _polygon polygon;


float polygon_area(polygon *p);

bool polygon_pointin(polygon *p, point *f);

bool polygon_overlapping(polygon *p, polygon *q);

void polygon_rotate(polygon *p, float t);

void polygon_minbox(polygon *p, float *minx, float *miny, float *maxx, float *maxy);

void polygon_translate(polygon *p, float x, float y);

#endif
