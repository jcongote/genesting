/*!\mainpage Projecto Genesting

Genesting es un projecto que intenta resolver el problema de nesting o
anidamiento de figuras a través del uso de algoritmos genéticos.

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

/*!\defgroup distance Distancias
Para el projecto utilizamos la definicion de distancia Euclidiana, donde la
distancia entre dos objetos es la longitud de la recta mas cercana que los toca.
*/

/*!\defgroup geometry Geometria
Son los modelos y estructuras geometricas utilizadas para calcular las
caracteristicas del problema, debido a que nesting esta fuertemente enlazado
con poligonos, es necesario implementar el conjunto de objetos geometricos
que lo manejen.
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

    population_create(p,g, 10);

    int i,j,k;
    for (k=0;k<20;k++)
    {
        for (i=0;i<10;i++)
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

        population_evaluate(p);

        for (i=0;i<10;i++)
        {
            printf("Individuo %i: [%i] fitness: %f\n",i,p->individuos[i].ngenes,p->individuos[i].fitness);
            for (j=0;j<p->individuos[i].ngenes;j++)
            {
                printf("Pat[%i] x[%f] y[%f] t[%f]\n",
                       p->individuos[i].posgen[j].id,
                       p->individuos[i].posgen[j].x,
                       p->individuos[i].posgen[j].y,
                       p->individuos[i].posgen[j].t);
            }
        }

        population_generation(p);
    }
    free(p);

    return 0;
}
