#ifndef FILE_LINE_H
#define FILE_LINE_H

#include "point.h"

struct _line
{
    point v1,v2;
};

typedef struct _line line;

int line_LineIntersection(line *l1, line *l2);

float distance_pointLine(point *p, line *l);

#endif
