/*!\addtogroup distance
@{
*/

/*!\file distance.h*/

#ifndef FILE_DISTANCE_H
#define FILE_DISTANCE_H

#include "point.h"
#include "line.h"
#include "polygon.h"
#include "polygon_holes.h"

float distance_pointpoint(point *a, point *b);

float distance_pointline(point *f, line *l,int *seg);

float distance_pointpolygon(point *f, polygon *p, line *ref);

float distance_pointpolygonholes(point *f, polygon_holes *p, line *ref);

#endif

/*!@}*/
