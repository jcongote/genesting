
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
        if (polygon_pointinPolygon(q,&p->v[i]))
            return 1;
    }

    for (i=0;i<q->nvertices;i++)
    {
        if (polygon_pointinPolygon(p,&q->v[i]))
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
        if (i==0 || min>d)
        {
            min = d;
        }
    }
    return min;
}

void rotate_Polygon(polygon *p, float t)
{
    int i;
    float maxx,maxy;
    float minx,miny;

    float mx,my;

    float sent = sin(t);
    float cost = cos(t);

    minbox_Polygon(p, &minx, &miny, &maxx, &maxy);

    mx = ((maxx - minx)/2)+minx;
    my = ((maxy - miny)/2)+miny;

    for (i=0;i<p->nvertices;i++)
    {
        float lx= p->v[i].x - mx;
        float ly= p->v[i].y - my;

        p->v[i].x=(lx*cost - ly * sent)+mx;

        p->v[i].y=(ly*cost + lx * sent)+my;

        if (i==0)
        {
            minx = p->v[i].x;
            miny = p->v[i].y;
        }
        else
        {
            if (minx > p->v[i].x)
            {
                minx = p->v[i].x;
            }
            if (miny > p->v[i].y)
            {
                miny = p->v[i].y;
            }
        }
    }
}


void minbox_Polygon(polygon *p, float *x1, float *y1, float *x2, float *y2){
    int i;
    for (i=0;i<p->nvertices;i++)
    {
        if (i==0)
        {
            *x1 = p->v[i].x;
            *y1 = p->v[i].y;

            *x2 = p->v[i].x;
            *y2 = p->v[i].y;
        }
        else
        {
            if (*x1 > p->v[i].x)
            {
                *x1 = p->v[i].x;
            }
            if (*y1 > p->v[i].y)
            {
                *y1 = p->v[i].y;
            }
            if (*x2 < p->v[i].x)
            {
                *x2 = p->v[i].x;
            }
            if (*y2 < p->v[i].y)
            {
                *y2 = p->v[i].y;
            }
        }
    }
}


void translate_Polygon(polygon *p, float x, float y){
    int i;
    for (i=0;i<p->nvertices;i++)
    {
        p->v[i].x-=x;
        p->v[i].y-=y;
    }
}
