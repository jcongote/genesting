#include <stdlib.h>

#include "individuo.h"

#include "polygon.h"
#include "polygon_holes.h"

/*!\fn float individuo_fitness(individuo *ind)
Esta funcion evalua que tan bueno es un individuo en la poblacion. retorna
un valor entre 0 y 1, y entre mas cercano a 0, mejor es el individuo
*/
float individuo_fitness(individuo *ind)
{
    int i,j;

    polygon_holes temp;

    temp.h = (polygon*) malloc(sizeof(polygon)*(ind->ngenes+ind->ambiente->nhuecos));

    temp.nholes = ind->ngenes+ind->ambiente->nhuecos;

    for (i=0;i<ind->ngenes+ind->ambiente->nhuecos;i++)
    {
        if (i<ind->ngenes)
        {
            temp.h[i].nvertices = ind->ambiente->patrones[ind->posgen[i].id].nvertices;
            temp.h[i].v=(point*) malloc(sizeof(point)*temp.h[i].nvertices);

            for (j=0;j<temp.h[i].nvertices;j++)
            {
                temp.h[i].v[j].x=ind->ambiente->patrones[ind->posgen[i].id].v[j].x;
                temp.h[i].v[j].y=ind->ambiente->patrones[ind->posgen[i].id].v[j].y;
            }

            polygon_rotate(&(temp.h[i]),ind->posgen[i].t);
            polygon_translate(&(temp.h[i]),-ind->posgen[i].x, -ind->posgen[i].y);
        }
        else
        {
            temp.h[i].nvertices = ind->ambiente->huecos[i-ind->ngenes].nvertices;
            temp.h[i].v=(point*) malloc(sizeof(point)*temp.h[i].nvertices);
            for (j=0;j<temp.h[i].nvertices;j++)
            {
                temp.h[i].v[j].x=ind->ambiente->huecos[i-ind->ngenes].v[j].x;
                temp.h[i].v[j].y=ind->ambiente->huecos[i-ind->ngenes].v[j].y;
            }
        }
    }

    ind->fitness = polygonholes_volumen(&temp)/(ind->ambiente->volumen);

    ind->areautil = polygonholes_area(&temp);

    for (i=0;i<ind->ngenes+ind->ambiente->nhuecos;i++)
    {
        free(temp.h[i].v);
    }
    free (temp.h);

    return (ind->fitness);
}

individuo* individuo_create(genesting *g){
        individuo *ind;
        float maxx,maxy,minx,miny;

        ind = (individuo*) malloc (sizeof(individuo));
        srand((int)ind);

        ind->ambiente = g;
        ind->ngenes = 1;

        ind->posgen = malloc (sizeof(posicion));

        polygon_minbox(&(g->plantilla), &minx, &miny, &maxx, &maxy);

        ind->posgen->x = (rand()%(int)(maxx-minx))+minx;
        ind->posgen->y = (rand()%(int)(maxy-miny))+miny;
        ind->posgen->t = (rand()%628)/100;
        ind->posgen->id= rand()%g->npatrones;

        return ind;
}

void individuo_mutate(individuo *i);

individuo* individuo_procreate(individuo *p, individuo *m);

bool individuo_validate(individuo *i);
