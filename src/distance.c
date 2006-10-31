/*!\addtogroup distance
@{
*/

/*!\file distance.c
En este archivo se definen las diferentes funciones para encontrar la distancia
entre varios objetos.
*/

#include <stdlib.h>
#include <math.h>
#include <float.h>

#include "distance.h"

#include "point.h"
#include "line.h"
#include "polygon.h"


/*!\fn float distance_pointline(point *f, line *l,int *seg)
La funcion calcula la distancia entre un punto un segmento de linea,
se pueden dar dos casos:
    - La perpendicular del punto a la recta intersecta el segmento de recta, en
    esta caso la distancia es la longitud del punto al punto de interseccion de
    la recta con la perpendicular que pasa por el punto

    - La perpendicular del punto a la recta no intersecta el segmente, en este
    caso la distancia entre el punto y la recta es igual a la minima distancia
    entre el punto y los puntos extremos del segmento de recta.

\param [in] f Punto en 2 dimensiones
\param [in] l Segmento de recta que tiene definido los dos puntos extremos
\param [out] seg Indica cual fue la refencia para tomar la distancia puede tomar
                 tres valores:0 cuando la distancia es respecto a la
                 perpendicular de la linea, 1 cuando la distancia es respecto al
                 punto extremo 1 y 2 cuando la distancia es respecto al punto
                 extremo 2
\return La distancia entre el segmento de recta y el punto
*/
float distance_pointline(point *f, line *l,int *seg)
{
    if(point_dot(&(l->v1),&(l->v2),f) > 0)
    {
        *seg = 2;
        return distance_pointpoint(&(l->v2),f);
    }

    if(point_dot(&(l->v2),&(l->v1),f) > 0)
    {
        *seg =1;
        return distance_pointpoint(&(l->v1),f);
    }

    *seg =0;
    return fabsf(point_cross(&(l->v1),&(l->v2),f) / distance_pointpoint(&(l->v1),&(l->v2)));
}

/*!\fn float distance_pointpolygon(point *f, polygon *p, line *ref)
La funcion calcula la distancia entre un punto y el borde de un poligono simple,
para esto calcula la distancia del punto contra todos los segmentos de linea
que conforma el poligono y retorna la menor distancia, esta funcion no tiene
consideracion si el punto esta adentro o afuera del poligono porque calcula
la distancia al borde.

\param [in] f Punto en 2 dimensiones
\param [in] p Poligono simple
\param [out] ref Escribe en la memoria el segmento de recta que define la
                 menor distancia entre el poligono y el punto, si la distancia
                 fue tomada respecto a un punto extremo, entonces el segmento
                 de recta es un punto, el extremo.
\return La distancia entre el punto y el poligono
*/
float distance_pointpolygon(point *f, polygon *p, line *ref)
{
    int seg,i;
    float min=FLT_MAX;

    for (i=0;i<p->nvertices;i++)
    {
        float d;
        line l;

        l.v1=p->v[i];
        l.v2=p->v[(i+1)%(p->nvertices)];
        d=distance_pointline(f,&l,&seg);
        if (i==0 || min>d)
        {
            min = d;
            if (ref!=NULL)
            {
                switch (seg)
                {
                    case 0:
                    ref->v1 = l.v1;
                    ref->v2 = l.v2;
                    break;
                    case 1:
                    ref->v1 = l.v1;
                    ref->v2 = l.v1;
                    break;
                    case 2:
                    ref->v1 = l.v2;
                    ref->v2 = l.v2;
                    break;
                }
            }
        }
    }
    return min;
}

/*!\fn float distance_pointpolygonholes(point *f, polygon_holes *p, line *ref)
La funcion calcula la distancia entre un punto y un poligono con huecos,
para esto calcula la distancia del punto con el borde del poligono y
posteriormente con cada uno de los huecos de este, seleccionado la distancia
minima
\param [in] f Punto en 2 dimensiones
\param [in] p Poligono con huecos
\param [out] ref Escribe en la memoria el segmento de recta que define la
                 menor distancia entre el poligono y sus huecos con el punto,
                 si la distancia fue tomada respecto a un punto extremo,
                 entonces el segmento de recta es un punto, el extremo.
\return La distancia entre un punto y un poligono con huecos
*/
float distance_pointpolygonholes(point *f, polygon_holes *p, line *ref)
{
    int i;
    float min;

    min = distance_pointpolygon(f,p->p,ref);
    for (i=0;i<p->nholes;i++)
    {
        float min2;
        line ref2;
        min2 = distance_pointpolygon(f,&(p->h[i]),&ref2);
        if (min2 < min)
        {
            min = min2;
            if (ref!=NULL)
            {
                ref->v1 = ref2.v1;
                ref->v2 = ref2.v2;
            }
        }
    }
    return min;
}
/*!@}*/
