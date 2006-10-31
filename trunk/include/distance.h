/*!\defgroup distance Distancias
Para el projecto utilizamos la definicion de distancia Euclidiana, donde la
distancia entre dos objetos es la longitud de la recta mas cercana que los toca.
*/

/*!\addtogroup distance
@{
*/

/*!\file distance.h*/

#ifndef FILE_DISTANCE_H
#define FILE_DISTANCE_H

#include "geometry.h"

float distance_pointline(point *f, line *l,int *seg);

float distance_pointpolygon(point *f, polygon *p, line *ref);

float distance_pointpolygonholes(point *f, polygon_holes *p, line *ref);

#endif

/*!@}*/
