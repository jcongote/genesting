#include <stdlib.h>
#include <math.h>

#include "distance.h"

#include "point.h"
#include "line.h"
#include "polygon.h"

float distance_pointpoint(point *a, point *b) //OK
{
    return sqrt(powf(a->x - b->x,2.0)+powf(a->y - b->y,2.0));
}

/*
Retorna la distancia entre un punto y un segmento de linea

seg:
0: cuando la distancia es respecto a la perpendicular de la linea
1: cuando la distancia es respecto al punto extremo 1
2: cuando la distancia es respecto al punto extremo 2
*/
float distance_pointline(point *f, line *l,int *seg)
{
    float dist,dot1,dot2;

    dist = point_cross(&(l->v1),&(l->v2),f) / distance_pointpoint(&(l->v1),&(l->v2));
    dot1 = point_dot(&(l->v1),&(l->v2),f);

    if(dot1 > 0)
    {
        *seg = 2;
        return distance_pointpoint(&(l->v2),f);
    }

    dot2 = point_dot(&(l->v2),&(l->v1),f);

    if(dot2 > 0)
    {
        *seg =1;
        return distance_pointpoint(&(l->v1),f);
    }

    *seg =0;
    return fabsf(dist);
}

float distance_pointpolygon(point *f, polygon *p, line *ref) //OK
{
    int seg,i;
    float min;

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
            if (ref!=NULL){
                ref->v1 = ref2.v1;
                ref->v2 = ref2.v2;
            }
        }
    }

    return min;
}

float foo(){
    float min=40;
        return min;
}
