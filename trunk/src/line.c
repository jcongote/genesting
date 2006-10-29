/*!\addtogroup geometry
@{
*/

/*!\file line.c
Este archivo define las funciones que operan sobre el objeto segmento de linea
*/

#include <math.h>

#include "line.h"

/*!\fn bool line_intersection(line *l1, line *l2)
Identifica si dos lineas se intersectan, el algoritmo utilizado fue sacado
del libro de Graphics Gems del Articulo Faster Line Segment Intersection

\param [in] l1 Linea en 2 dimensiones
\param [in] l2 Linea en 2 dimensiones
\return Un valor booleano que es verdadero si las lineas se intersectan y falso
en caso contrario
*/
bool line_intersection(line *l1, line *l2)
{
    float x1,y1,x2,y2,x3,y3,x4,y4;
    float Ax,Bx,Cx,Ay,By,Cy;
    float x1lo,x1hi,y1lo,y1hi;
    float d,e,f;

    x1 = l1->v1.x;
    y1 = l1->v1.y;

    x2 = l1->v2.x;
    y2 = l1->v2.y;

    x3 = l2->v1.x;
    y3 = l2->v1.y;

    x4 = l2->v2.x;
    y4 = l2->v2.y;

    Ax = x2 - x1;
    Bx = x3 - x4;

    if (Ax < 0){
        x1lo=x2;
        x1hi=x1;
    } else {
        x1hi=x2;
        x1lo=x1;
    }

    if (Bx > 0){
            if (x1hi < x4 || x2 < x1lo) return false;
    } else {
            if (x1hi < x3 || x4 < x1lo) return false;
    }

    Ay = y2 - y1;
    By = y3 - y4;

    if (Ay < 0){
            y1lo = y2;
            y1hi = y1;
    } else {
            y1hi = y2;
            y1lo = y1;
    }

    if (By > 0){
            if (y1hi < y4 || y3 < y1lo) return false;
    } else {
            if (y1hi < y3 || y4 < y1lo) return false;
    }

    Cx = x1 - x3;
    Cy = y1 - y3;

    d = By*Cx - Bx*Cy;
    f = Ay*Bx - Ax*By;

    if (f>0){
            if (d<0 || d>f) return false;
    } else {
            if (d>0 || d<f) return false;
    }

    e = Ax*Cy - Ay*Cx;

    if (f>0){
            if (e<0 || e>f) return false;
    } else {
            if (e>0 || e<f) return false;
    }

    if (f==0) return false;

    return true;
}

/*!\fn bool line_equal(line *l1, line *l2)
Compara dos segmentos de linea, en este caso el orden de los extremos
importa en la operacion

\param [in] l1 Linea en 2 dimensiones
\param [in] l2 Linea en 2 dimensiones
\return Verdadero si los segmentos de recta son iguales,
                falso en caso contrario
*/
bool line_equal(line *l1, line *l2)
{
    return (l1->v1.x == l2->v1.x &&
            l1->v1.y == l2->v1.y &&
            l1->v2.x == l2->v2.x &&
            l1->v2.y == l2->v2.y
           ) ? true : false;
}

/*!\fn bool line_ispoint(line *l)
Identifica si un segmento de recta es en realidad un unico punto, esto
lo hace comprobando los puntos extremos del segmento de recta, si los
extremos son iguales entonces se puede definir este segmento de recta
como una linea

\param [in] l Linea en 2 dimensiones
\return Verdadero si la linea es un punto, falso en caso contrario.
*/

bool line_ispoint(line *l)
{
    return (l->v1.x == l->v2.x && l->v1.y == l->v2.y) ? true : false;
}

/*!@}*/
