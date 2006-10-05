#ifndef FILE_LINE_H
#define FILE_LINE_H

#include <stdbool.h>

#include "point.h"

struct _line
{
    point v1,v2;
};

typedef struct _line line;

bool line_intersection(line *l1, line *l2);

bool line_equal(line *l1, line *l2);

bool line_ispoint(line *l1);

#endif
