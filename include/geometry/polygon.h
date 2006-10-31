/*!\addtogroup geometry
@{
*/

/*!\file polygon.h
Declaraciones del Objeto Poligono
*/

#ifndef FILE_POLYGON_H
#define FILE_POLYGON_H

#include <stdbool.h>

#include "geometry.h"

typedef struct _polygon* polygon;

float polygon_area(polygon p);

bool polygon_pointin(polygon p, point f);

bool polygon_polygonin(polygon p, polygon q);

bool polygon_overlapping(polygon p, polygon q);

float polygon_cx(polygon p);

float polygon_cy(polygon p);


/*
Esta funcion en vez de cambiar el poligono deberia regresar uno nueva
ademas con esto se puede evitar un recalculo de algunos elementos ademas
se deben unir, haciendo una funcion que rote y translade simultaneamente
*/
//void polygon_rotate(polygon p, float t);
//void polygon_translate(polygon p, float x, float y);


/*
Esta caracteristica no es util fuera del contexto de encontrar el volumen
por lo tanto puede se embedida dentro del codigo que calcula el volumen
*/
//void polygon_minbox(polygon p, float *minx, float *miny, float *maxx, float *maxy);



#endif

/*!@}*/
