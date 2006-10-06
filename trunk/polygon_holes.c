/*!\addtogroup geometry
@{
*/

/*!\file polygon_holes.c
Este archivo define las funciones que se utilizan en el Objeto Poligono con
huecos
*/

#include <stdlib.h>
#include <math.h>

#include "polygon_holes.h"

#include "polygon.h"
#include "distance.h"

#include "graphics.h"


/*!\fn float polygonholes_area(polygon_holes *p)
La funcion calcula el area de un Poligono con huecos, para hacer esto
calculamos el area del poligono formado por el borde exterior y le restamos
el area de cada uno de los huecos.

\param [in] p Poligono simple en 2 dimensiones
\return Area del poligono simple con huecos
*/
float polygonholes_area(polygon_holes *p)
{
    unsigned int i;
    float area;
    area = polygon_area(p->p);
    for (i=0;i<p->nholes;i++)
    {
        area -= polygon_area(&(p->h[i]));
    }
    return area;
}

/*!\fn float polygonholes_volumen(polygon_holes *p)
Calcula el volumen generado por el poligono con huecos, definiendo asi el
poligono como una figura tridimensional de la siguiente manera, la base del
solido esta conformada por el poligono con huecos y la altura del solido
en cada punto es igual a la distancia del punto a uno de los bordes.

El metodo para calcular el volumen se basa en encerrar el poligono en un
rectangulo que lo contenga, y sucesivamente partir el poligono en dos
subrectangulos respecto al lado mas largo calculando el volumen de cada
uno de los subrectangulos y sumando el valor y esto se hace de manera recursiva.

    - Cuando el subrectangulo esta completamente fuera del poligono no
      calculamos el volumen y regresamos un valor de 0

    - Cuando el subrectangulo tiene un area muy pequena definida por un da
      se calcula el centro del subrectangulo y se verifica que este dentro
      del poligono, si este es el caso, entonces se devuelve el valor del
      area multiplicada por la distancia del punto medio al borde, si el
      punto medio es 0 se devuelve 0.

    - Cuando el subrectangulo esta dentro del poligono y los vertices de este
      tienen su menor distancia respecto a un mismo segmento de recta se
      calcula el volumen del poliedro formado como el promedio de las distancias
      de los vertices multiplicados por el area del subrectangulo.

    - Cuando el subrectangulo esta dentro del poligono y los vertices de este
      tienen su menor distancia respecto a un mismo punto, que se da en el
      caso de los poligonos concavos, entonces se calcula el volumen como el
      volumen formado por la interseccion entre el cono que genera el punto y
      el subrectangulo, la formula que da este volumen es:
      \f$\int_{a}^{b}\int_{c}^{d}\sqrt{((x-h)^2 + (y-k)^2)} dx dy\f$ donde
      \f$(h,k)\f$ son las coordenadas del punto con respecto al que se toma la
      distancia, \f$(a,b)\f$ son los valores en que se desplaza \f$y\f$,
      \f$(c,d)\f$ son los valores en que se desplaza \f$x\f$.

Esta funcion evalua menos puntos si en ves de dividir el rectangulo en 2 segun
su lado mas largo, se divide en 4 partes iguales, pero esta aproximacion puede
tener problemas cuando la proporcion del los lados del subrectangulo es muy
grande.

\param [in] p Poligono simple con huecos en 2 dimensiones
\return Volumen generado
*/

float polygonholes_volumen(polygon_holes *p)
{
    void draw_polygon(polygon *p)
    {
        int i;
        for (i=0;i<p->nvertices;i++)
        {
            draw_line(p->v[i].x,p->v[i].y,p->v[(i+1)%p->nvertices].x,p->v[(i+1)%p->nvertices].y);
        }
    }

    float polygonholes_volumen_box( polygon_holes *p,
                                    float minx,
                                    float miny,
                                    float maxx,
                                    float maxy)
    {
#if graphics
        draw_polygon(p->p);
#endif

        float vol=0;
        float da;

        polygon rec;
        point q[4];

        q[0].x = minx;
        q[0].y = miny;

        q[1].x = maxx;
        q[1].y = miny;

        q[2].x = maxx;
        q[2].y = maxy;

        q[3].x = minx;
        q[3].y = maxy;

        rec.v =(point *) &q;
        rec.nvertices = 4;

        da = polygon_area(&rec);


        if (!polygon_overlapping(p->p, &rec)) //OK
        {
            vol = 0;
#if graphics

            draw_rect(minx, miny, maxx, maxy,255,255,255);
#endif

        }
        else if (da < DELTA) // OK
        {
            point pm;

            pm.x=minx+((maxx-minx)/2.0);
            pm.y=miny+((maxy-miny)/2.0);

            if (polygonholes_pointin(p, &pm))
            {
                vol = da * distance_pointpolygonholes(&pm, p,NULL);

#if graphics

                draw_rect(minx, miny, maxx, maxy,128,128,128);
#endif

            }
            else
            {
                vol = 0;
#if graphics

                draw_rect(minx, miny, maxx, maxy,225,255,255);
#endif

            }
        }
        else
        {

            int i;
            float dis[4];
            line ref[4];
            for (i=0;i<4;i++)
            {
                dis[i]=distance_pointpolygonholes(&q[i], p,&ref[i]);
            }

            if( polygonholes_polygonin(p, &rec)
                    &&
                    (line_equal(&ref[0],&ref[1]) &&
                     line_equal(&ref[1],&ref[2]) &&
                     line_equal(&ref[2],&ref[3]))

              ) // OK
            {
                if (line_ispoint(&ref[0]))// Si es un punto entonces se utiliza la formula del cono
                {
                    double a,b,c,d,h,k;
                    c=minx;
                    d=maxx;
                    a=miny;
                    b=maxy;
                    h=ref[0].v1.x;
                    k=ref[0].v1.y;
                    vol = (2*a*c*sqrt(pow(-c + h,2) + pow(a - k,2)) - 2*a*h*sqrt(pow(-c + h,2) + pow(a - k,2)) - 2*a*d*sqrt(pow(-d + h,2) + pow(a - k,2)) + 2*a*h*sqrt(pow(-d + h,2) + pow(a - k,2)) - 2*b*c*sqrt(pow(-c + h,2) + pow(b - k,2)) + 2*b*h*sqrt(pow(-c + h,2) + pow(b - k,2)) + 2*b*d*sqrt(pow(-d + h,2) + pow(b - k,2)) - 2*b*h*sqrt(pow(-d + h,2) + pow(b - k,2)) - 2*c*sqrt(pow(-c + h,2) + pow(a - k,2))*k + 2*h*sqrt(pow(-c + h,2) + pow(a - k,2))*k + 2*d*sqrt(pow(-d + h,2) + pow(a - k,2))*k - 2*h*sqrt(pow(-d + h,2) + pow(a - k,2))*k + 2*c*sqrt(pow(-c + h,2) + pow(b - k,2))*k - 2*h*sqrt(pow(-c + h,2) + pow(b - k,2))*k - 2*d*sqrt(pow(-d + h,2) + pow(b - k,2))*k + 2*h*sqrt(pow(-d + h,2) + pow(b - k,2))*k - pow(a,3)*log(-c + h + sqrt(pow(-c + h,2) + pow(a - k,2))) + 3*pow(a,2)*k*log(-c + h + sqrt(pow(-c + h,2) + pow(a - k,2))) - 3*a*pow(k,2)*log(-c + h + sqrt(pow(-c + h,2) + pow(a - k,2))) + pow(k,3)*log(-c + h + sqrt(pow(-c + h,2) + pow(a - k,2))) + pow(a,3)*log(-d + h + sqrt(pow(-d + h,2) + pow(a - k,2))) - 3*pow(a,2)*k*log(-d + h + sqrt(pow(-d + h,2) + pow(a - k,2))) + 3*a*pow(k,2)*log(-d + h + sqrt(pow(-d + h,2) + pow(a - k,2))) - pow(k,3)*log(-d + h + sqrt(pow(-d + h,2) + pow(a - k,2))) + pow(b,3)*log(-c + h + sqrt(pow(-c + h,2) + pow(b - k,2))) - 3*pow(b,2)*k*log(-c + h + sqrt(pow(-c + h,2) + pow(b - k,2))) + 3*b*pow(k,2)*log(-c + h + sqrt(pow(-c + h,2) + pow(b - k,2))) - pow(k,3)*log(-c + h + sqrt(pow(-c + h,2) + pow(b - k,2))) - pow(b,3)*log(-d + h + sqrt(pow(-d + h,2) + pow(b - k,2))) + 3*pow(b,2)*k*log(-d + h + sqrt(pow(-d + h,2) + pow(b - k,2))) - 3*b*pow(k,2)*log(-d + h + sqrt(pow(-d + h,2) + pow(b - k,2))) + pow(k,3)*log(-d + h + sqrt(pow(-d + h,2) + pow(b - k,2))) + pow(c,3)*log(a + sqrt(pow(-c + h,2) + pow(a - k,2)) - k) - 3*pow(c,2)*h*log(a + sqrt(pow(-c + h,2) + pow(a - k,2)) - k) + 3*c*pow(h,2)*log(a + sqrt(pow(-c + h,2) + pow(a - k,2)) - k) - pow(h,3)*log(a + sqrt(pow(-c + h,2) + pow(a - k,2)) - k) - pow(d,3)*log(a + sqrt(pow(-d + h,2) + pow(a - k,2)) - k) + 3*pow(d,2)*h*log(a + sqrt(pow(-d + h,2) + pow(a - k,2)) - k) - 3*d*pow(h,2)*log(a + sqrt(pow(-d + h,2) + pow(a - k,2)) - k) + pow(h,3)*log(a + sqrt(pow(-d + h,2) + pow(a - k,2)) - k) - pow(c,3)*log(b + sqrt(pow(-c + h,2) + pow(b - k,2)) - k) + 3*pow(c,2)*h*log(b + sqrt(pow(-c + h,2) + pow(b - k,2)) - k) - 3*c*pow(h,2)*log(b + sqrt(pow(-c + h,2) + pow(b - k,2)) - k) + pow(h,3)*log(b + sqrt(pow(-c + h,2) + pow(b - k,2)) - k) + pow(d,3)*log(b + sqrt(pow(-d + h,2) + pow(b - k,2)) - k) - 3*pow(d,2)*h*log(b + sqrt(pow(-d + h,2) + pow(b - k,2)) - k) + 3*d*pow(h,2)*log(b + sqrt(pow(-d + h,2) + pow(b - k,2)) - k) - pow(h,3)*log(b + sqrt(pow(-d + h,2) + pow(b - k,2)) - k))/6;
#if graphics

                    draw_rect(minx, miny, maxx, maxy,0,128,0);
#endif

                }
                else // Si es una linea se utiliza la formula de la cuna
                {
                    vol = (dis[0]+dis[1]+dis[2]+dis[3])/4.0 * da;
#if graphics

                    draw_rect(minx, miny, maxx, maxy,128,0,0);
#endif

                }
            }
            else
            {


                if (!(
                            polygonholes_pointinhole(p, &q[0]) &&
                            polygonholes_pointinhole(p, &q[1]) &&
                            polygonholes_pointinhole(p, &q[2]) &&
                            polygonholes_pointinhole(p, &q[3])
                        )) // el poligono esta dentro de p pero no en un hueco
                {


                    //Opcion de corte 1
                    /*
                                        vol+= polygonholes_volumen_box(p, minx, miny, (minx+maxx)/2.0, (miny+maxy)/2.0);
                                        vol+= polygonholes_volumen_box(p, (minx+maxx)/2.0, miny, maxx, (miny+maxy)/2.0);
                                        vol+= polygonholes_volumen_box(p, minx, (miny+maxy)/2.0, (minx+maxx)/2.0, maxy);
                                        vol+= polygonholes_volumen_box(p, (minx+maxx)/2.0, (miny+maxy)/2.0, maxx, maxy);
                    */


                    if (maxx-minx > maxy-miny)
                    {
                        vol+= polygonholes_volumen_box(p, minx, miny, (minx+maxx)/2.0, maxy);
                        vol+= polygonholes_volumen_box(p, (minx+maxx)/2.0, miny, maxx, maxy);
                    }
                    else
                    {
                        vol+= polygonholes_volumen_box(p, minx, miny, maxx, (miny+maxy)/2.0);
                        vol+= polygonholes_volumen_box(p, minx, (miny+maxy)/2.0, maxx, maxy);
                    }


                }
#if graphics
                else
                {
                    draw_rect(minx, miny, maxx, maxy,0,0,255);
                }
#endif
            }
        }
        return vol;
    }

    float minx, miny, maxx, maxy;
    polygon_minbox(p->p, &minx, &miny, &maxx, &maxy);
    return polygonholes_volumen_box(p,minx,miny,maxx,maxy);
}

/*!\fn bool polygonholes_pointin(polygon_holes *p, point *f)
Esta funcion identifica si un punto esta dentro de un poligono con huecos
por lo tanto evalua inicialmente que el punto este dentro del poligono
exterior y posterioremente que no se encuentre dentro de los huecos

\param [in] p Poligono simple con huecos en 2 dimensiones
\param [in] f Punto en 2 dimensiones
\return Verdadero en caso que el punto este dentro del poligono, falso en
        caso contrario
*/
bool polygonholes_pointin(polygon_holes *p, point *f)
{
    bool valido=false;
    if (polygon_pointin(p->p, f))
    {
        unsigned int i;
        valido=true;

        for (i=0;i<p->nholes && valido;i++)
        {
            if (polygon_pointin(&(p->h[i]), f))
            {
                valido=false;
            }
        }
    }
    return valido;
}

/*!\fn bool polygonholes_pointinhole(polygon_holes *p, point *f)
Identifica si un punto esta dentro de un hueco del poligono.

\param [in] p Poligono Simple con huecos
\param [in] f Punto en 2 dimensiones
\return Verdadero si el punto esta dentro de uno de los huecos, falso en
caso contrario
*/

bool polygonholes_pointinhole(polygon_holes *p, point *f)
{
    bool valido=false;
    unsigned int i;

    for (i=0;i<p->nholes && !valido;i++)
    {
        if (polygon_pointin(&(p->h[i]), f))
        {
            valido=true;
        }
    }
    return valido;
}


/*!\fn bool polygonholes_polygonin(polygon_holes *p, polygon *q)
La funcion evalua que el poligono q esta completamente adentro del poligono p
entonces todos los puntos de q deben estar dentro de p y no se deben intersectar
las lineas que los conforman

\param [in] p Poligono Simple con huecos
\param [in] q Poligono Simple
\return Verdadero en caso del que poligono q este dentro del poligono p, falso
        en caso contrario
*/
bool polygonholes_polygonin(polygon_holes *p, polygon *q)
{
    int i,j,k;
    bool adentro = true;

    for (i=0;i<q->nvertices && adentro;i++)
    {
        if (!polygonholes_pointin(p,&q->v[i]))
            adentro = false;
    }

    for (i=0;i<q->nvertices && adentro;i++)
    {
        line t1,t2;
        t1.v1=q->v[i];
        t1.v2=q->v[(i+1)%(q->nvertices)];

        for (j=0;j<p->p->nvertices && adentro;j++)
        {
            t2.v1=p->p->v[j];
            t2.v2=p->p->v[(j+1)%p->p->nvertices];
            if (line_intersection(&t1,&t2))
                adentro = false;
        }

        for (j=0;j<p->nholes && adentro; j++)
        {
            for (k=0;k<p->h[j].nvertices && adentro; k++)
            {
                t2.v1 = p->h[j].v[k];
                t2.v2 = p->h[j].v[(k+1)%p->h[j].nvertices];
                if (line_intersection(&t1,&t2))
                    adentro = false;
            }
        }
    }
    return adentro;
}

/*!@}*/
