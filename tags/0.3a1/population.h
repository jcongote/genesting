/*!\addtogroup genetic
@{
*/

/*!\file population.h
Declaraciones del Objeto Poblacion
*/

#ifndef FILE_POPULATION_H
#define FILE_POPULATION_H

#include "individuo.h"
#include "genesting.h"

struct _population{
    genesting *ambiente;
    int nindividuos;
    individuo *individuos;
};

typedef struct _population population;

void population_create(population *p, genesting *g, int n);

void population_evaluate(population *p);

void population_generation(population *p);

#endif

/*!@}*/
