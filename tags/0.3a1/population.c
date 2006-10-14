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


/*!\fn void population_create(population *p, genesting *g, int n)
Esta funcion crea una poblacion inicial de soluciones.

\param [out] p Poblacion
\param [in] g Genesting
\param [in] n Numero de elementos de la poblacion
*/
void population_create(population *p, genesting *g, int n){
    int i;
    p->ambiente=g;
    p->nindividuos=n;
    p->individuos = (individuo*) malloc(sizeof(individuo)*p->nindividuos);

    for (i=0;i<p->nindividuos;i++){
            individuo_create(p->ambiente,&(p->individuos[i]));
    }
};


/*!\fn void population_evaluate(population *p)
Evalua una poblacion, para esto calcula todos los fitness,
y ordena los individuos segun su fitness

\param [in,out] p Poblacion
*/
void population_evaluate(population *p){
    int i;
    for (i=0;i<p->nindividuos;i++){
            p->individuos[i].fitness = individuo_fitness(&p->individuos[i]);
    }
    qsort(p->individuos,p->nindividuos,sizeof(individuo),(int(*)(const void*,const void*))comparar_individuos);
};


/*!\fn void population_generation(population *p)
Crea una nueva poblacion, dividiendo la exitente en tres grupos, dejando
el primer grupo intacto, el segundo lo muta, y el tercero es un cruce de
los 2 primeros
\param [in,out] p Poblacion
*/
void population_generation(population *p){
    int i;
    for (i=0;i<p->nindividuos/3;i++){
        individuo_procreate(&(p->individuos[i]),&(p->individuos[(p->nindividuos/3)+i]),&(p->individuos[(2*p->nindividuos/3)+i]));
        individuo_mutate(&(p->individuos[(p->nindividuos/3)+i]));
    }
    for (i=0;i<p->nindividuos;i++){
        if (!individuo_validate(&(p->individuos[i]))){
            individuo_create(p->ambiente,&(p->individuos[i]));
        }
    }
};

/*!@}*/
