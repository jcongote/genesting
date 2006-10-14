/*!\addtogroup genetic
@{
*/

/*!\file individuo.c
Aqui estan las estructuras principales del programa
la estructura es muy sencilla
*/

#include <stdlib.h>

#include "individuo.h"

#include "polygon.h"
#include "polygon_holes.h"

/*!\fn float individuo_fitness(individuo *ind)
Esta funcion evalua que tan bueno es un individuo en la poblacion. retorna
un valor entre 0 y 1, y entre mas cercano a 0, mejor es el individuo
\param [in] ind Individuo
*/

/*!\todo
No es necesario recalcular el fitness de un individuo si este no ha cambiado
*/
float individuo_fitness(individuo *ind)
{
    int i,j,cont;

    polygon_holes temp;

    temp.p = &(ind->ambiente->plantilla);

    temp.h = (polygon*) malloc(sizeof(polygon)*(ind->ngenes+ind->ambiente->nhuecos));

    temp.nholes = ind->ngenes+ind->ambiente->nhuecos;

    for (i=0,cont=0;i<ind->ambiente->nhuecos;i++)
    {
        temp.h[cont].nvertices = ind->ambiente->huecos[i].nvertices;
        temp.h[cont].v=(point*) malloc(sizeof(point)*temp.h[i].nvertices);

        for (j=0;j<temp.h[cont].nvertices;j++)
        {
            temp.h[cont].v[j].x=ind->ambiente->huecos[i].v[j].x;
            temp.h[cont].v[j].y=ind->ambiente->huecos[i].v[j].y;
        }
        cont++;
    }
    for (i=0;i<ind->ngenes;i++)
    {
        temp.h[cont].nvertices = ind->ambiente->patrones[ind->posgen[i].id].nvertices;
        temp.h[cont].v = (point*) malloc(sizeof(point)*temp.h[cont].nvertices);
        for (j=0;j<temp.h[cont].nvertices;j++)
        {
            temp.h[cont].v[j].x=ind->ambiente->patrones[ind->posgen[i].id].v[j].x;
            temp.h[cont].v[j].y=ind->ambiente->patrones[ind->posgen[i].id].v[j].y;
        }
        polygon_rotate(&(temp.h[cont]),ind->posgen[i].t);
        polygon_translate(&(temp.h[cont]),ind->posgen[i].x, ind->posgen[i].y);
        cont++;
    }

    ind->fitness = polygonholes_volumen(&temp)/(ind->ambiente->volumen);

    ind->areautil = polygonholes_area(&temp);

    for (i=0;i<temp.nholes;i++)
    {
        free(temp.h[i].v);
    }
    free (temp.h);

    return (ind->fitness);
}

/*!\fn void individuo_create(genesting *g,individuo *ind)
Esta funcion crea un individuo de configuracion aleatoria.

El individuo con el ambiente definido en el objeto genesting y en
una posicion aleatoria entre el rectangulo creado por la plantilla,
Ademas se selecciona solo un patron que conforma inicialmente el
individuo, es posible que la configuracion obtenido genere un individuo
no valido, pero esto debera ser verificado posteriormente por el algoritmo
genetico.

\param [in] g Obtiene el contexto del individuo
\param [out] ind Una direccion de memoria donde esta el individuo
*/

void individuo_create(genesting *g,individuo *ind)
{
    float maxx,maxy,minx,miny;

    ind->ambiente = g;
    ind->ngenes = 1;
    ind->posgen = (posicion*) malloc (sizeof(posicion));
    polygon_minbox(&(g->plantilla), &minx, &miny, &maxx, &maxy);

    do{
    ind->posgen->x = (rand()%(int)(maxx-minx))+minx;
    ind->posgen->y = (rand()%(int)(maxy-miny))+miny;
    ind->posgen->t = (rand()%628)/100.0;
    ind->posgen->id= rand()%g->npatrones;
    } while (!individuo_validate(ind));
}

/*!\fn void individuo_mutate(individuo *ind)
Esta funcion modifica un individuo aleatoriamente.

\param [in,out] ind Individuo
*/
void individuo_mutate(individuo *ind)
{
    int i;
    for (i=0;i<ind->ngenes;i++){
        ind->posgen[i].x += (rand()%4)-2;
        ind->posgen[i].y += (rand()%4)-2;
        ind->posgen[i].t += ((rand()%100)-50)/100.0;
    }
};

/*!\fn void individuo_procreate(individuo *p, individuo *m, individuo *ind)
Esta funcion crea un nuevo individuo partiendo de los individuos
padres. La mescla se hace de la siguiente manera, se toman los
patrones del padre y luego los patrones de la madre si ya no
estan ya colocados, si hay patrones comunes de forma cuasi aleatoria
se escoge cual de las posiciones del patron es heredada.

\param [in] p Individuo padre
\param [in] m Individuo madre
\param [out] ind Nuevo individuo
*/
void individuo_procreate(individuo *p, individuo *m, individuo *ind)
{
    int i,j,cont;

    ind->posgen = (posicion*) realloc(ind->posgen,sizeof(posicion)*(p->ngenes+m->ngenes));

    for (i=0,cont=0;i<p->ngenes;i++,cont++)
    {
        ind->posgen[cont].x = p->posgen[i].x;
        ind->posgen[cont].y = p->posgen[i].y;
        ind->posgen[cont].t = p->posgen[i].t;
        ind->posgen[cont].id = p->posgen[i].id;
    }

    for (i=0;i<m->ngenes;i++)
    {
        bool nuevo=true;
        for (j=0;j<p->ngenes && nuevo;j++)
        {
            if (m->posgen[i].id == p->posgen[j].id)
            {
                nuevo = false;
                if ((i+j)%2==0)
                {
                    ind->posgen[j].x = m->posgen[i].x;
                    ind->posgen[j].y = m->posgen[i].y;
                    ind->posgen[j].t = m->posgen[i].t;
                }
            }
        }
        if (nuevo)
        {
            ind->posgen[cont].x = m->posgen[i].x;
            ind->posgen[cont].y = m->posgen[i].y;
            ind->posgen[cont].t = m->posgen[i].t;
            ind->posgen[cont].id = m->posgen[i].id;
            cont++;
        }
    }
    ind->ambiente = p->ambiente;
    ind->ngenes = cont;
    ind->posgen = (posicion*) realloc(ind->posgen,sizeof(posicion)*cont);
}


/*!\fn bool individuo_validate(individuo *ind)
Esta funcion identifica si un individuo es una solucion validad, esto es
verdadero si se cumplen las siguientes condiciones:
    - No se repite ningun patron dentro de la solucion
    - Todos los patrones estan dentro de la plantilla
    - Ningun patron se solapa con la plantilla, huecos u otro patron
En caso contrario el individuo no es valido.

\param [in] ind Individuo a validar
\return Verdadero si es un individuo valido, falso en caso contrario.
*/
bool individuo_validate(individuo *ind)
{
    bool valido = true;
    int i,j;
    polygon_holes ph;
    polygon *pat;

    for (i=0;i<ind->ngenes-1 && valido;i++)
    {
        for (j=i+1;j<ind->ngenes && valido;j++)
        {
            if (ind->posgen[i].id == ind->posgen[j].id)
                valido = false;
        }
    }

    ph.p = &(ind->ambiente->plantilla);
    ph.nholes = ind->ambiente->nhuecos;
    ph.h = (polygon*) malloc(sizeof(polygon)*ph.nholes);

    for (i=0;i<ph.nholes;i++)
    {
        ph.h[i].nvertices = ind->ambiente->huecos[i].nvertices;
        ph.h[i].v=(point*) malloc(sizeof(point)*ph.h[i].nvertices);

        for (j=0;j<ph.h[i].nvertices;j++)
        {
            ph.h[i].v[j].x=ind->ambiente->huecos[i].v[j].x;
            ph.h[i].v[j].y=ind->ambiente->huecos[i].v[j].y;
        }
    }

    pat = (polygon*) malloc(sizeof(polygon)*ind->ngenes);

    for (i=0;i<ind->ngenes;i++)
    {
        pat[i].nvertices = ind->ambiente->patrones[ind->posgen[i].id].nvertices;
        pat[i].v = (point*) malloc(sizeof(point)*pat[i].nvertices);
        for (j=0;j<pat[i].nvertices;j++)
        {
            pat[i].v[j].x=ind->ambiente->patrones[ind->posgen[i].id].v[j].x;
            pat[i].v[j].y=ind->ambiente->patrones[ind->posgen[i].id].v[j].y;
        }
        polygon_rotate(&(pat[i]),ind->posgen[i].t);
        polygon_translate(&(pat[i]),ind->posgen[i].x, ind->posgen[i].y);
    }

    for (i=0;i<ind->ngenes && valido;i++)
    {
        if(!polygonholes_polygonin(&ph, &(pat[i])))
        {
            valido=false;
        }
    }

    for (i=0;i<ind->ngenes-1 && valido;i++)
    {
        for (j=i+1;j<ind->ngenes && valido;j++)
        {
            if (polygon_overlapping(&(pat[i]),&(pat[j])))
            {
                valido=false;
            }
        }
    }

    for (i=0;i<ind->ngenes;i++)
    {
        free(pat[i].v);
    }
    free(pat);

    for (i=0;i<ph.nholes;i++)
    {
        free(ph.h[i].v);
    }
    free(ph.h);

    return valido;
}

/*!\fn int comparar_individuos(individuo *ind1,individuo *ind2)
Compara entre los dos individuos cual es mejor segun su fitness
*/

int comparar_individuos(individuo *ind1,individuo *ind2)
{
    return ((int)(((ind1->fitness)-(ind2->fitness))*10000.0));
}

/*!@}*/
