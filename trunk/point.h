/*!\addtogroup geometry
@{
*/

/*!\file  point.h
Implementacion de un objeto punto en 2 dimensiones
*/

#ifndef FILE_POINT_H
#define FILE_POINT_H

/*!\struct _point point.h point.c
Define un punto en un espacio de dos dimensiones
*/
struct _point
{
    /*!\var x Coordenada x del punto*/
    float x;

     /*!\var y Coordenada y del punto*/
    float y;
};
typedef struct _point point;

float point_dot(point *a, point *b, point *c);

float point_cross(point *a, point *b, point *c);

#endif

/*!@}*/
