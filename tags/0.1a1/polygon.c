
#include <math.h>

#include "polygon.h"

float polygon_Area(polygon *p) //OK
{
    unsigned int i,j,n;
    float area = 0;

    n = p->nvertices;

    for (i=0;i<n;i++)
    {
        j = (i + 1) % n;
        area += p->v[i].x * p->v[j].y;
        area -= p->v[i].y * p->v[j].x;
    }
    area /= 2;

    return(area < 0 ? -area : area);
}

int polygon_Overlapping(polygon *p, polygon *q)
{
    int i,j;

    for (i=0;i<p->nvertices;i++)
    {
        if (polygon_pointinPolygon(q,&p[i]))
            return 1;
    }

    for (i=0;i<q->nvertices;i++)
    {
        if (polygon_pointinPolygon(p,&q[i]))
            return 1;
    }

    for (i=0;i<=p->nvertices;i++)
    {
        line t1,t2;
        t1.v1=p->v[i];
        t1.v2=p->v[(i+1)%(p->nvertices)];
        for (j=0;j<=q->nvertices;j++)
        {
            t2.v1=q->v[j];
            t2.v2=q->v[(j+1)%q->nvertices];
            if (line_LineIntersection(&t1,&t2))
                return 1;
        }
    }
    return 0;
}

int polygon_pointinPolygon(polygon *p, point *f) //OK
{
    int c=0;
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



float distance_pointPolygon(point *p, polygon *q) //OK
{
    int i;
    float min;

    for (i=0;i<q->nvertices;i++)
    {
        float d;
        line l;

        l.v1=q->v[i];
        l.v2=q->v[(i+1)%(q->nvertices)];
        d=distance_pointLine(p,&l);
        if (i==0 || min>d){
            min = d;
        }
    }
    return min;
}
