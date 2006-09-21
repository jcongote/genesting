#ifndef FILE_POLYGON_H
#define FILE_POLYGON_H

#include "point.h"
#include "line.h"


struct _polygon
{
    unsigned int nvertices;
    point *v;
};

typedef struct _polygon polygon;


float polygon_Area(polygon *p);

int polygon_Overlapping(polygon *p, polygon *q);

int polygon_pointinPolygon(polygon *p, point *f);

float distance_pointPolygon(point *p, polygon *q);

#endif
