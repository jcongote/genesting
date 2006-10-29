/*!\addtogroup geometry
@{
*/

/*!\file polygon.c
Este archivo define las funciones utilizadas por el objeto poligono
*/

#include <math.h>

#include "polygon.h"

/*!\fn float polygon_area(polygon *p)
Esta funcion calcula el area de un poligono simple, el algoritmo fue tomado
de Graphic Gems II, del articulo The Area of a Simple Polygon.

\param [in] p Poligono simple
\return El area del poligono
*/
float polygon_area(polygon *p)
{
    unsigned int i;
    float area = 0;
    for (i=0;i<p->nvertices;i++)
    {
        area += p->v[i].x * p->v[(i + 1) % p->nvertices].y;
        area -= p->v[i].y * p->v[(i + 1) % p->nvertices].x;
    }
    area /= 2;

    return(area < 0 ? -area : area);
}


/*!\fn bool polygon_pointin(polygon *p, point *f)
La funcion identifica si un punto esta dentro de un poligono o no.

\param [in] p Poligono simple en 2 dimensiones
\param [in] f Punto en 2 dimensiones
\return Un booleano que es verdadero en caso de que el punto este dentro del
        poligono y falso en caso contrario
*/
/*
la implementacion y explicacion de este codigo esta en la pagina
http://www.faqs.org/faqs/graphics/algorithms-faq
*/

bool polygon_pointin(polygon *p, point *f)
{
    bool c=false;
    unsigned int i,j;

    for (i=0,j=p->nvertices-1;i<p->nvertices;j=i++)
    {
        if ((((p->v[i].y<=f->y) && (f->y<p->v[j].y)) ||
                ((p->v[j].y<=f->y) && (f->y<p->v[i].y))) &&
                (f->x < (p->v[j].x - p->v[i].x) * (f->y - p->v[i].y) / (p->v[j].y - p->v[i].y) + p->v[i].x))
            c = !c;
    }
    return c;
}

/*!\fn bool polygon_overlapping(polygon *p, polygon *q)
Esta funcion identifica si dos poligonos se solapan de alguna forma para
verificar esto se realizan 3 comprobaciones:
    - Ningun vertice del poligono p esta dentro del poligono q
    - Ningun vertice del poligono q esta dentro del poligono p
    - Ninguna linea de los poligonos p y q se intersectan

\param [in] p Poligono Simple en 2 dimensiones
\param [in] q Poligono Simple en 2 dimensiones
\return Verdadero si los poligonos se solapan, falso en caso contrario.
*/
bool polygon_overlapping(polygon *p, polygon *q)
{
    int i,j;

    for (i=0;i<p->nvertices;i++)
    {
        if (polygon_pointin(q,&p->v[i]))
            return true;
    }

    for (i=0;i<q->nvertices;i++)
    {
        if (polygon_pointin(p,&q->v[i]))
            return true;
    }

    for (i=0;i<p->nvertices;i++)
    {
        line t1,t2;
        t1.v1=p->v[i];
        t1.v2=p->v[(i+1)%(p->nvertices)];
        for (j=0;j<q->nvertices;j++)
        {
            t2.v1=q->v[j];
            t2.v2=q->v[(j+1)%q->nvertices];
            if (line_intersection(&t1,&t2))
                return true;
        }
    }
    return false;
}

/*!\fn void polygon_rotate(polygon *p, float t)
La funcion rota el poligono p un angulo t, teniendo como eje de rotacion el
centro de la caja mas pequena que contiene el poligono. Aunque se pueden
elegir otros puntos como eje, o de hecho implementar un eje arbitrario, el
centro de la caja que contiene el poligono es un punto facil de calcular y
ademas que evita exageradas translaciones relativas del poligono al ser rotado.

\param [in,out] p Poligono simple
\param [in] t Angulo en radianes a rotar la figura en sentido antihorario.
*/
void polygon_rotate(polygon *p, float t)
{
    int i;
    point pc;

    float sent = sin(t);
    float cost = cos(t);

    pc = polygon_center(p);

    polygon_translate(p, -pc.x, -pc.y);

    for (i=0;i<p->nvertices;i++)
    {
        p->v[i].x=(p->v[i].x*cost - p->v[i].y * sent);
        p->v[i].y=(p->v[i].y*cost + p->v[i].x * sent);
    }

    polygon_translate(p, pc.x, pc.y);
}

point polygon_center(polygon *p)
{
    int i=0;
    float minx, maxx, miny, maxy;

    point r;

    polygon_minbox(p, &minx, &miny, &maxx, &maxy)

    r.x = ((maxx - minx)/2)+minx;
    r.y = ((maxy - miny)/2)+miny;

    return r;
}

/*!\fn void polygon_minbox(polygon *p, float *minx, float *miny, float *maxx, float *maxy)
La funcion calcula las coordenadas mas extremas del poligono, conformando con
estas 2 puntos que pueden formar un rectangulo que contiene completamente el
poligono

\param [in] p Poligono simple
\param [out] minx Coordenada mas pequeña del poligono en el eje x
\param [out] miny Coordenada mas pequeña del poligono en el eje y
\param [out] maxx Coordenada mas grande del poligono en el eje x
\param [out] maxy Coordenada mas grande del poligono en el eje y
*/
void polygon_minbox(polygon *p, float *minx, float *miny, float *maxx, float *maxy)
{
    int i;

    *minx = p->v[0].x;
    *miny = p->v[0].y;
    *maxx = p->v[0].x;
    *maxy = p->v[0].y;

    for (i=1;i<p->nvertices;i++)
    {
        if (*minx > p->v[i].x)
        {
            *minx = p->v[i].x;
        }
        if (*miny > p->v[i].y)
        {
            *miny = p->v[i].y;
        }
        if (*maxx < p->v[i].x)
        {
            *maxx = p->v[i].x;
        }
        if (*maxy < p->v[i].y)
        {
            *maxy = p->v[i].y;
        }
    }
}

/*!\fn void polygon_translate(polygon *p, float x, float y)
Translada el poligono una distancia definida por x,y.

\param [in,out] p Poligono simple
\param [in] x Distancia en x a transladar el poligono
\param [in] y Distancia en y a transladar el poligono
*/
void polygon_translate(polygon *p, float x, float y)
{
    int i;
    for (i=0;i<p->nvertices;i++)
    {
        p->v[i].x+=x;
        p->v[i].y+=y;
    }
}

/*!@}*/
