/*!\addtogroup genetic
@{
*/

/*!\file genesting.c
Aqui estan las estructuras principales del programa
la estructura es muy sencilla
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "genesting.h"

#include "polygon.h"
#include "polygon_holes.h"

/*!\fn genesting* leer_archivo(char *arc_name)
Lee el archivo de datos que definen las caracteristicas del problema
*/
/*!\todo
Documentar como es la estructura leida por el programa.
*/

genesting* leer_archivo(char *arc_name)
{
    FILE* arc;
    genesting *g;
    int npoly;
    int i,j;

    g =(genesting*) malloc (sizeof(genesting));

    arc=fopen(arc_name,"r");

    if(arc==NULL){
            fprintf(stderr,"No se pudo abrir el archivo\n");
            exit(1);
    }

    fscanf(arc,"%i",&npoly);

    g->nhuecos =0;
    g->npatrones =0;

    g->huecos=(polygon*) malloc(sizeof(polygon)*npoly-1);
    g->patrones=(polygon*) malloc(sizeof(polygon)*npoly-1);

    for (i=0;i<npoly;i++)
    {
        int nvert,tipo;

        polygon *p;
        fscanf(arc,"%i %i",&nvert,&tipo);
        switch(tipo)
        {
            case 1:
            p=&(g->plantilla);
            break;
            case 2:
            p=&(g->patrones[g->npatrones++]);
            break;
            case 3:
            p=&(g->huecos[g->nhuecos++]);
            break;
        }
        p->nvertices = nvert;
        p->v=(point*) malloc(sizeof(point)*nvert);
        for (j=0;j<nvert;j++)
        {
            fscanf(arc,"%f %f",&(p->v[j].x),&(p->v[j].y));
        }
    }
    g->huecos=(polygon*) realloc(g->huecos,sizeof(polygon)*g->nhuecos);
    g->patrones=(polygon*) realloc(g->patrones,sizeof(polygon)*g->npatrones);

    fclose(arc);

    genesting_init(g);

    return g;
}
/*!\fn void genesting_init(genesting *g)
Realiza unas correcciones iniciales al archivo de entrada y calcula los
valores fijos del problema, como el area maxima y volumen maximo que se
dan cuando generamos una solucion vacia.
*/
void genesting_init(genesting *g)
{
    int i;
    float minx, miny, maxx, maxy;
    polygon_holes p;

    polygon_minbox(&(g->plantilla), &minx, &miny, &maxx, &maxy);

    polygon_translate(&(g->plantilla), -minx, -miny);

    for (i=0;i<g->nhuecos;i++)
    {
        polygon_translate(&(g->huecos[i]), -minx, -miny);
    }

    for (i=0;i<g->npatrones;i++)
    {
        polygon_minbox(&(g->patrones[i]), &minx, &miny, &maxx, &maxy);
        polygon_translate(&(g->patrones[i]), -minx, -miny);
    }

    p.nholes = g->nhuecos;
    p.p = &(g->plantilla);
    p.h = g->huecos;

    g->area = polygonholes_area(&p);

    g->volumen = polygonholes_volumen(&p);
}

/*!\fn float fitness(individuo *ind)
Esta funcion evalua que tan bueno es un individuo en la poblacion. retorna
un valor entre 0 y 1, y entre mas cercano a 0, mejor es el individuo
*/

float fitness(individuo *ind)
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

/*!@}*/