/*!\addtogroup geometry
@{
*/

/*!\file polygon_holes.h
Declaraciones del Objeto Poligono con huecos
*/

#ifndef FILE_POLYGON_HOLES_H
#define FILE_POLYGON_HOLES_H

#include <stdbool.h>

#include "point.h"
#include "polygon.h"

struct _polygon_holes
{
    unsigned int nholes;
    polygon *p;
    polygon *h;
};

typedef struct _polygon_holes polygon_holes;

float polygonholes_area(polygon_holes *p);

float polygonholes_volumen(polygon_holes *p);

bool polygonholes_pointin(polygon_holes *p, point *f);

bool polygonholes_polygonin(polygon_holes *p, polygon *q);

bool polygonholes_pointinhole(polygon_holes *p, point *f);

#endif

/*!@}*/
