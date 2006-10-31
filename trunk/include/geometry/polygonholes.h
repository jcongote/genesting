/*!\addtogroup geometry
@{
*/

/*!\file polygon_holes.h
Declaraciones del Objeto Poligono con huecos
*/

#ifndef FILE_POLYGON_HOLES_H
#define FILE_POLYGON_HOLES_H

#include <stdbool.h>

#include <geometry.h>

typedef struct _polygonholes* polygonholes;

float polygonholes_area(polygonholes p);

float polygonholes_volumen(polygonholes p);

bool polygonholes_pointin(polygonholes p, point f);

bool polygonholes_polygonin(polygonholes p, polygon q);

#endif

/*!@}*/
