/*!\addtogroup genetic
@{
*/

/*!\file individuo.h
Declaraciones del Objeto Individuo
*/

#ifndef FILE_INDIVIDUO_H
#define FILE_INDIVIDUO_H

#include "genesting.h"

struct _posicion;
struct _individuo;

typedef struct _posicion posicion;

typedef struct _individuo individuo;

struct _posicion
{
    float x;
    float y;
    float t;
    unsigned int id;
};

struct _individuo
{
    genesting *ambiente;
    posicion *posgen;

    unsigned int ngenes;
    float fitness;
    float areautil;
};

float individuo_fitness(individuo *ind);

void individuo_create(genesting *g,individuo *ind);

void individuo_mutate(individuo *ind);

void individuo_procreate(individuo *p, individuo *m, individuo *ind);

bool individuo_validate(individuo *ind);

int comparar_individuos(individuo *ind1,individuo *ind2);

#endif
/*!@}*/
