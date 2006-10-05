
#include <math.h>

#include "polygon.h"

float polygon_area(polygon *p) //OK
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

bool polygon_pointin(polygon *p, point *f) //OK
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

void polygon_rotate(polygon *p, float t)
{
    int i;
    float maxx,maxy;
    float minx,miny;

    float mx,my;

    float sent = sin(t);
    float cost = cos(t);

    polygon_minbox(p, &minx, &miny, &maxx, &maxy);

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

void polygon_minbox(polygon *p, float *minx, float *miny, float *maxx, float *maxy){
    int i;
    for (i=0;i<p->nvertices;i++)
    {
        if (i==0)
        {
            *minx = p->v[i].x;
            *miny = p->v[i].y;

            *maxx = p->v[i].x;
            *maxy = p->v[i].y;
        }
        else
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
}

void polygon_translate(polygon *p, float x, float y){
    int i;
    for (i=0;i<p->nvertices;i++)
    {
        p->v[i].x+=x;
        p->v[i].y+=y;
    }
}
