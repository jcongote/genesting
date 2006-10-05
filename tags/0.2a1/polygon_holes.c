#include <stdlib.h>
#include <math.h>

#define DELTA 0.1

#include "polygon_holes.h"

#include "polygon.h"
#include "distance.h"

#include "graphics.h"

static float polygonholes_volumen_box( polygon_holes *p,
                                       float minx,
                                       float miny,
                                       float maxx,
                                       float maxy);

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

float polygonholes_volumen(polygon_holes *p)
{
    float minx, miny, maxx, maxy;
    polygon_minbox(p->p, &minx, &miny, &maxx, &maxy);
    return polygonholes_volumen_box(p,minx,miny,maxx,maxy);
}

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


/*
El poligono q esta completamente adentro del poligono p

entonces todos los puntos de q deben estar dentro de p y
no se deben intersectar las lineas
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


static float polygonholes_volumen_box( polygon_holes *p,
                                       float minx,
                                       float miny,
                                       float maxx,
                                       float maxy)
{
    void draw_polygon(polygon *p)
    {
        int i;
        for (i=0;i<p->nvertices;i++)
        {
            draw_line(p->v[i].x,p->v[i].y,p->v[(i+1)%p->nvertices].x,p->v[(i+1)%p->nvertices].y);
        }
    }
    draw_polygon(p->p);

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
    }
    else if (da < DELTA) // OK
    {
        point pm;

        pm.x=minx+((maxx-minx)/2.0);
        pm.y=miny+((maxy-miny)/2.0);

        if (polygonholes_pointin(p, &pm))
        {
            vol = da * distance_pointpolygonholes(&pm, p,NULL);
        }
        else
        {
            vol = 0;
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

            draw_rect(minx, miny, maxx, maxy,255,255,255);
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
            }
            else // Si es una linea se utiliza la formula de la cuna
            {
                vol = (dis[0]+dis[1]+dis[2]+dis[3])/4.0 * da;
            }
        }
        else
        {
            draw_rect(minx, miny, maxx, maxy,255,255,0);

            if (!(
                        polygonholes_pointinhole(p, &q[0]) &&
                        polygonholes_pointinhole(p, &q[1]) &&
                        polygonholes_pointinhole(p, &q[2]) &&
                        polygonholes_pointinhole(p, &q[3])
                    )) // el poligono esta dentro de p pero no en un hueco
            {
                vol+= polygonholes_volumen_box(p, minx, miny, (minx+maxx)/2.0, (miny+maxy)/2.0);
                vol+= polygonholes_volumen_box(p, (minx+maxx)/2.0, miny, maxx, (miny+maxy)/2.0);
                vol+= polygonholes_volumen_box(p, minx, (miny+maxy)/2.0, (minx+maxx)/2.0, maxy);
                vol+= polygonholes_volumen_box(p, (minx+maxx)/2.0, (miny+maxy)/2.0, maxx, maxy);
            }
        }
    }
    return vol;
}

