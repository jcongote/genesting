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
            polygon_translate(&(temp.h[i]),ind->posgen[i].x, ind->posgen[i].y);
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

/*!\fn individuo* individuo_create(genesting *g)
Esta funcion crea un individuo de configuracion aleatoria, retornado
el puntero del individuo, donde el programador debe manejar la memoria.

El individuo con el ambiente definido en el objeto genesting y en
una posicion aleatoria entre el rectangulo creado por la plantilla,
Ademas se selecciona solo un patron que conforma inicialmente el
individuo, es posible que la configuracion obtenido genere un individuo
no valido, pero esto debera ser verificado posteriormente por el algoritmo
genetico.

\param [in] g Obtiene el contexto del individuo
\return Una direccion de memoria donde esta el individuo
*/

individuo* individuo_create(genesting *g)
{
    individuo *ind;
    float maxx,maxy,minx,miny;

    ind = (individuo*) malloc (sizeof(individuo));
    srand((int)ind);

    ind->ambiente = g;
    ind->ngenes = 1;

    ind->posgen = (posicion*) malloc (sizeof(posicion));

    polygon_minbox(&(g->plantilla), &minx, &miny, &maxx, &maxy);

    ind->posgen->x = (rand()%(int)(maxx-minx))+minx;
    ind->posgen->y = (rand()%(int)(maxy-miny))+miny;
    ind->posgen->t = (rand()%628)/100;
    ind->posgen->id= rand()%g->npatrones;

    return ind;
}

/*!\fn void individuo_mutate(individuo *ind)
Esta funcion modifica un individuo aleatoriamente.

\param [in,out] ind Individuo
*/
void individuo_mutate(individuo *ind)
{
    ind->posgen->x += (rand()%4)-2;
    ind->posgen->y += (rand()%4)-2;
    ind->posgen->t += ((rand()%100)-50)/100;
};


/*!\fn individuo* individuo_procreate(individuo *p, individuo *m)
Esta funcion crea un nuevo individuo partiendo de los individuos
padres. La mescla se hace de la siguiente manera, se toman los
patrones del padre y luego los patrones de la madre si ya no
estan ya colocados, si hay patrones comunes de forma cuasi aleatoria
se escoge cual de las posiciones del patron es heredada.

\param [in] p Individuo padre
\param [in] m Individuo madre
\return Nuevo individuo
*/
individuo* individuo_procreate(individuo *p, individuo *m)
{
    individuo *ind = (individuo*) malloc (sizeof(individuo));
    int i,j,cont;

    ind->posgen = (posicion*) malloc (sizeof(posicion)*(p->ngenes+m->ngenes));

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

    return ind;
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

    for (i=0;i<ind->ngenes-1 && valido;i++){
            for (j=i+1;j<ind->ngenes && valido;j++){
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
                ph.h[i].v=ind->ambiente->huecos[i].v;
            }
    }

    pat = (polygon*) malloc(sizeof(polygon)*ind->ngenes);

    for (i=0;i<ind->ngenes;i++){
            pat[i].nvertices = ind->ambiente->patrones[ind->posgen[i].id].nvertices;
            pat[i].v = (point*) malloc(sizeof(point)*pat[i].nvertices);
            for (j=0;j<pat[i].nvertices;j++)
            {
                pat[i].v=ind->ambiente->patrones[ind->posgen[i].id].v;
            }
            polygon_rotate(&(pat[i]),ind->posgen[i].t);
            polygon_translate(&(pat[i]),ind->posgen[i].x, ind->posgen[i].y);
    }

    for (i=0;i<ind->ngenes && valido;i++){
        if(!polygonholes_polygonin(&ph, &(pat[i]))){
            valido=false;
        }
    }

    for (i=0;i<ind->ngenes-1 && valido;i++){
            for (j=i+1;j<ind->ngenes && valido;j++){
                    if (polygon_overlapping(&(pat[i]),&(pat[j]))){
                            valido=false;
                    }
            }
    }

    for (i=0;i<ind->ngenes;i++){
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

/*!@}*/
