/*!\addtogroup genetic
@{
*/

/*!\file population.c
Definiciones del Objeto Poblacion
*/

#include <stdlib.h>

#include "population.h"
#include "genesting.h"
#include "individuo.h"

void population_create(population *p, genesting *g, int n){
    int i;
    p->ambiente=g;
    p->nindividuos=n;
    p->individuos = (individuo*) malloc(sizeof(individuo)*p->nindividuos);

    for (i=0;i<p->nindividuos;i++){
            individuo_create(p->ambiente,&(p->individuos[i]));
    }
};

void population_evaluate(population *p){
    int i;
    for (i=0;i<p->nindividuos;i++){
            p->individuos[i].fitness = individuo_fitness(&p->individuos[i]);
    }
    qsort(p->individuos,p->nindividuos,sizeof(individuo),(int(*)(const void*,const void*))comparar_individuos);
};

void population_generation(population *p){
    int i;
    for (i=0;i<p->nindividuos/3;i++){
        individuo_procreate(&(p->individuos[i]),&(p->individuos[(p->nindividuos/3)+i]),&(p->individuos[(2*p->nindividuos/3)+i]));
        individuo_mutate(&(p->individuos[(p->nindividuos/3)+i]));
    }
    free(p->individuos[p->nindividuos-1].posgen);
    individuo_create(p->ambiente,&(p->individuos[p->nindividuos-1]));
};

/*!@}*/
