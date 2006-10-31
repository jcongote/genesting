/*!\addtogroup geometry
@{
*/

/*!\file polygon.c
Este archivo define las funciones utilizadas por el objeto poligono
*/

#include <math.h>

#include "geometry.h"

struct _polygon
{
    unsigned int nl;
    line *l;

    float area;
    float cx,cy;

    float minx;
    float miny;
    float maxx;
    float maxy;

};

polygon polygon_new(unsigned int np){

}

void polygon_addpoint(polygon p,unsigned int n, float x, float y)


/*!\fn float polygon_area(polygon p)
Esta funcion calcula el area de un poligono simple, el algoritmo fue tomado
de Graphic Gems II, del articulo The Area of a Simple Polygon.

\param [in] p Poligono simple
\return El area del poligono
*/
float polygon_area(polygon p)
{
    if (!isnan(p->area))
        return p->area;

    unsigned int i;
    for (i=0;i<p->nl;i++){
        p->area += line_area(p->l[i]);
    }
    p->area=fabsf(p->area);

    return p->area;
}


/*!\fn bool polygon_pointin(polygon p, point f)
La funcion identifica si un punto esta dentro de un poligono o no.

\param [in] p Poligono simple en 2 dimensiones
\param [in] f Punto en 2 dimensiones
\return Un booleano que es verdadero en caso de que el punto este dentro del
        poligono y falso en caso contrario
*/

bool polygon_pointin(polygon p, point f)
{
    unsigned int w,i;

    for (i=0,w=0;i<p->nl;i++){
        w+=line_pointabove(p->l[i], f);
    }
    return (w==0 ? false : true);
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
bool polygon_overlapping(polygon p, polygon q)
{
    int i,j;

    for (i=0;i<p->nl;i++)
    {
        if (polygon_pointin(q,line_getpoint1(p->l[i])))
            return true;
    }

    for (i=0;i<q->nl;i++)
    {
        if (polygon_pointin(p,line_getpoint1(q->l[i])))
            return true;
    }

    for (i=0;i<p->nl;i++)
    {
        for (j=0;j<q->nl;j++)
        {
            if (line_intersection(p->l[i],q->l[j]))
                return true;
        }
    }
    return false;
}


static void polygon_center(polygon p)
{
    unsigned int i;
    p->cx=0;
    p->cy=0;

    for (i=0;i<p->nl;i++){
        p->cx += line_cxpartial(p->l[i]);
        p->cy += line_cypartial(p->l[i]);
    }

    p->cx /= (6.0*polygon_area(p));
    p->cy /= (6.0*polygon_area(p));
}

float polygon_getcx(polygon p)
{
    if (isnan(p->cx))
        polygon_center(p);

    return p->cx;
}

float polygon_getcy(polygon p)
{
    if (isnan(p->cy))
        polygon_center(p);

    return p->cy;
}

/*!\fn void polygon_minbox(polygon p)
La funcion calcula las coordenadas mas extremas del poligono, conformando con
estas 2 puntos que pueden formar un rectangulo que contiene completamente el
poligono

\param [in] p Poligono simple
*/
static void polygon_minbox(polygon p)
{
    unsigned int i;

    p->minx = point_getx(p->l[0]);
    p->miny = point_gety(p->l[0]);
    p->maxx = point_getx(p->l[0]);
    p->maxy = point_gety(p->l[0]);

    for (i=1;i<p->nl;i++)
    {
        if (p->minx > point_getx(p->l[i]))
        {
            p->minx = point_getx(p->l[i]);
        }
        if (p->miny > point_gety(p->l[i]))
        {
            p->miny = point_gety(p->l[i]);
        }
        if (p->maxx < point_getx(p->l[i]))
        {
            p->maxx = point_getx(p->l[i]);
        }
        if (p->maxy < point_gety(p->l[i]))
        {
            p->maxy = point_gety(p->l[i]);
        }
    }
}

float polygon_getminx(polygon p){
        if (isnan(p->minx)){
            polygon_minbox(polygon p);
        }
        return p->minx;
}

float polygon_getmaxx(polygon p){
        if (isnan(p->maxx)){
            polygon_minbox(polygon p);
        }
        return p->maxx;
}

float polygon_getminy(polygon p){
        if (isnan(p->miny)){
            polygon_minbox(polygon p);
        }
        return p->miny;
}

float polygon_getmaxy(polygon p){
        if (isnan(p->maxy)){
            polygon_minbox(polygon p);
        }
        return p->maxy;
}

polygon polygon_getminbox(polygon p){
        polygon box;
        box = polygon_new(4);
        polygon_addpoint(p,0,p->minx,p->miny);
        polygon_addpoint(p,1,p->maxx,p->miny);
        polygon_addpoint(p,2,p->maxx,p->maxy);
        polygon_addpoint(p,3,p->minx,p->maxy);
        return box;
}


/*
Identifica si el poligono q esta dentro de p
*/
bool polygon_polygonin(polygon p, polygon q){

    int i,j;

    for (i=0;i<p->nl;i++)
    {
        if (polygon_pointin(q,line_getpoint1(p->l[i])))
            return false;
    }

    for (i=0;i<q->nl;i++)
    {
        if (!polygon_pointin(p,line_getpoint1(q->l[i])))
            return false;
    }

    for (i=0;i<p->nl;i++)
    {
        for (j=0;j<q->nl;j++)
        {
            if (line_intersection(p->l[i],q->l[j]))
                return false;
        }
    }
    return true;
}


/*!\fn void polygon_translate(polygon p, float x, float y)
Translada el poligono una distancia definida por x,y.

\param [in,out] p Poligono simple
\param [in] x Distancia en x a transladar el poligono
\param [in] y Distancia en y a transladar el poligono
*/
void polygon_translate(polygon p, float x, float y)
{
    /*
    int i;
    for (i=0;i<p->nvertices;i++)
    {
        p->v[i].x+=x;
        p->v[i].y+=y;
    }
    */
}

/*!\fn void polygon_rotate(polygon p, float t)
La funcion rota el poligono p un angulo t, teniendo como eje de rotacion el
centro de la caja mas pequena que contiene el poligono. Aunque se pueden
elegir otros puntos como eje, o de hecho implementar un eje arbitrario, el
centro de la caja que contiene el poligono es un punto facil de calcular y
ademas que evita exageradas translaciones relativas del poligono al ser rotado.

\param [in,out] p Poligono simple
\param [in] t Angulo en radianes a rotar la figura en sentido antihorario.
*/
void polygon_rotate(polygon p, float t)
{
    /*
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
    */
}


/*!@}*/
