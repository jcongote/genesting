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

individuo* individuo_create(genesting *g);

void individuo_mutate(individuo *ind);

individuo* individuo_procreate(individuo *p, individuo *m);

bool individuo_validate(individuo *ind);

#endif
/*!@}*/
