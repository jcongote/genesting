/*!\addtogroup geometry
@{
*/

/*!\file point.c
En este archivo definimos las funciones utilizadas por el objeto punto.
*/

#include <stdbool.h>
#include <stdlib.h>

#include "geometry.h"

struct _point
{
    float x;
    float y;
};

point point_new(float x, float y){
    point p= (point) malloc(sizeof(struct _point));
    p->x = x;
    p->y = y;
    return p;
}

void point_delete(point p){
    free(p);
}

void point_setx(point p, float x){
        p->x = x;
}

void point_sety(point p, float y){
        p->y = y;
}

float point_getx(point p){
    return p->x;
}

float point_gety(point p){
    return p->y;
}

bool point_equal(point a, point b){
        return (a->x == b->x && a->y == b->y);
}


/*!\fn float point_dot(point a, point b, point c)
Calcula el producto punto entre los puntos a, b y c. Aunque esta funcion
matematicamente esta definida en los vectores y no en los puntos, podemos
relacionar un punto como el vector que hay desde el origen del plano hasta
las coordenadas de este y asi calculamos el producto punto.

\param [in] a Punto en 2 dimensiones
\param [in] b Punto en 2 dimensiones
\param [in] c Punto en 2 dimensiones
\return El producto punto
*/
float point_dot(point a, point b, point c)
{
    return ((b->x - a->x) * (c->x - b->x)) + ((b->y - a->y) * (c->y - b->y));
}

/*!\fn float point_cross(point a, point b, point c)
Calcula el producto cruz entro los punto a, b y c. Aunque esta funcion
matematicamente esta definida en los vectores y no en los puntos, podemos
relacionar un punto como el vector que hay desde el origen del plano hasta
las coordenadas de este y asi calculamos el producto cruz.

\param [in] a Punto en 2 dimensiones
\param [in] b Punto en 2 dimensiones
\param [in] c Punto en 2 dimensiones
\return El producto cruz
*/
float point_cross(point a, point b, point c)
{
    return ((b->x - a->x)*(c->y - a->y)) - ((b->y - a->y)*(c->x - a->x));
}


/*!\fn float point_distance(point a, point b)
La funcion compara la distancia entre dos puntos la formula para esto es:
\f$\sqrt{(x_2 - x_1)^2 + (y_2 - y_1)^2}\f$

\param [in] a Punto en 2 dimensiones
\param [in] b Punto en 2 dimensiones
\return La distancia entre los puntos
*/
float point_distance(point a, point b)
{
    return sqrt(powf(a->x - b->x,2.0)+powf(a->y - b->y,2.0));
}

/*!@}*/
