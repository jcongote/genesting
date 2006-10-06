/*!\addtogroup geometry
@{
*/

/*!\file point.c
En este archivo definimos las funciones utilizadas por el objeto punto.
*/
#include "point.h"

/*!\fn float point_dot(point *a, point *b, point *c)
Calcula el producto punto entre los puntos a, b y c. Aunque esta funcion
matematicamente esta definida en los vectores y no en los puntos, podemos
relacionar un punto como el vector que hay desde el origen del plano hasta
las coordenadas de este y asi calculamos el producto punto.

\param [in] a Punto en 2 dimensiones
\param [in] b Punto en 2 dimensiones
\param [in] c Punto en 2 dimensiones
\return El producto punto
*/
float point_dot(point *a, point *b, point *c)
{
    return ((b->x - a->x) * (c->x - b->x)) + ((b->y - a->y) * (c->y - b->y));
}


/*!\fn float point_cross(point *a, point *b, point *c)
Calcula el producto cruz entro los punto a, b y c. Aunque esta funcion
matematicamente esta definida en los vectores y no en los puntos, podemos
relacionar un punto como el vector que hay desde el origen del plano hasta
las coordenadas de este y asi calculamos el producto cruz.

\param [in] a Punto en 2 dimensiones
\param [in] b Punto en 2 dimensiones
\param [in] c Punto en 2 dimensiones
\return El producto cruz
*/
float point_cross(point *a, point *b, point *c)
{
    return ((b->x - a->x)*(c->y - a->y)) - ((b->y - a->y)*(c->x - a->x));
}
/*!@}*/
