/*!\mainpage Proyecto Genesting
\author John Edgar Congote Calle
\version 0.3a1

Genesting es un proyecto que intenta resolver el problema de nesting o
anidamiento de figuras a través del uso de algoritmos genéticos.

\section Introduction

In the manufacturing industry the raw materials usually came in finite
two-dimensional sheets, where the permanent goal is the reduction of waste
materials. But there is a frequent problem: how to distribute two-dimensional
patterns in a container sheet in order to get the maximum utilization of
material? This is know as the Knapsack problem.


Nowdays many company make this job according with the empirical experience
of their employers having two risks. The first one is that there is not way
to know if their solution are going to be the best to minimize the amount of
waste materials. The second risk is in the case of presuming that an employ
has the optimal solution the knowledge would be in the hands of just one person
or group of work and not as a part of a system or as a part of the company.


\section definition Definicion
El problema de nesting se puede definir como encontrar una dispocision de
patrones que esten dentro de otro de forma que se maximice el area utilizada.
Este problema tiene muchas variantes, pero en el proyecto se trabajara
especificamente sobre el caso de Knapsack.

\section objective Objetivo
Encontrar una dispocicion de patrones que maximicen el area
utilizada dentro de otro patron. Los patrones estan definidos como poligonos
simples, los cuales pueden ser por definicion convexos o concavos pero solo
pueden tener un adentro o mas claramente sus lineas no se intersectan.

\section case Caso de Estudio
Aunque el problema es teorico, el projecto quiere generar una aplicacion
que pueda ser utilizada por la industria marroquinera, donde tienen que
definir como distribuir los moldes de corte dentro de un cuero, en este caso
el cuero se puede definir como el patron o poligono donde se tienen que
colocar los demas patrones, y los moldes como los patrones internos.
*/



/*!\defgroup genetic Genetico
Dentro del modulo genetico encontraremos una implementacion de heuristicas
de algoritmos geneticos y evoluticos que seleccionara la solucion.
*/


/*!\todo
- Se deben cambiar los typedef de los objetos para que sean punteros
- Para calcular el fitness es mejor comprobar primero el area y despues
el fitness, asi se ahorra mucho calculo
*/

/*!\file main.c
Programa principal, usa y prueba la libreria genesting.
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "genesting.h"
#include "graphics.h"
#include "distance.h"
#include "population.h"

int main(int argc, char **argv)
{
    genesting *g;
    population *p;

#if graphics

    init_graphics();
#endif

    if (argc<2)
    {
        fprintf(stderr,"Se deben entrar ingresar el nombre del archivo a leer\n");
        return 1;
    }

    g=leer_archivo(argv[1]);
    p=(population*) malloc(sizeof(population));

    srand((int)p);

    genesting_init(g);

    genesting_show(g);

    population_create(p,g, 200);

    int i,k;
    for (k=0;k<30;k++)
    {
        printf("Iteracion: %i\n",k);

        /*
        for (i=0;i<5;i++)
        {
            printf("Individuo %i: [%i] \n",i,p->individuos[i].ngenes);
            for (j=0;j<p->individuos[i].ngenes;j++)
            {
                printf("Pat[%i] x[%f] y[%f] t[%f]\n",
                       p->individuos[i].posgen[j].id,
                       p->individuos[i].posgen[j].x,
                       p->individuos[i].posgen[j].y,
                       p->individuos[i].posgen[j].t);
            }
        }
        */
        population_evaluate(p);

        for (i=0;i<10;i++)
        {
            printf("Individuo %i: [%i] fitness: %f areautil: %f\n",i,p->individuos[i].ngenes,p->individuos[i].fitness,p->individuos[i].areautil);
            /*
                        for (j=0;j<p->individuos[i].ngenes;j++)
                        {
                            printf("Pat[%i] x[%f] y[%f] t[%f]\n",
                                   p->individuos[i].posgen[j].id,
                                   p->individuos[i].posgen[j].x,
                                   p->individuos[i].posgen[j].y,
                                   p->individuos[i].posgen[j].t);
                        }
            */
        }

        population_generation(p);
    }
    free(p);

    return 0;
}
