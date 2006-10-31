/*!\addtogroup geometry
@{
*/

/*!\file  point.h
Implementacion de un objeto punto en 2 dimensiones
*/

#ifndef FILE_POINT_H
#define FILE_POINT_H

#include <stdbool.h>

typedef struct _point* point;

point point_new(float x, float y);

void point_delete(point p);

void point_setx(point p, float x);

void point_sety(point p, float y);

float point_getx(point p);

float point_gety(point p);

bool point_equal(point a, point b);

float point_dot(point a, point b, point c);

float point_cross(point a, point b, point c);

float point_distance(point a, point b);

#endif

/*!@}*/
