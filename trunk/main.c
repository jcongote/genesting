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

/*!\file main.c
Programa principal, usa y prueba la libreria genesting.
*/

#include <stdio.h>
#include <math.h>

#include "genesting.h"
#include "graphics.h"
#include "distance.h"

void show(genesting *g);

float f_volumen(  polygon *plantilla,
                  int nhuecos,
                  polygon *huecos,
                  int npatrones,
                  polygon *patrones,
                  float delta);

int main(int argc, char **argv)
{
    genesting *g;

#if graphics

    init_graphics();
#endif

    if (argc<2)
    {
        fprintf(stderr,"Se deben entrar ingresar el nombre del archivo a leer\n");
        return 1;
    }

    g=leer_archivo(argv[1]);

    show(g);
    getchar();
    return 0;
}

void show(genesting *g)
{
    int i;

    printf("Mostrando informacion de genesting\n");
    printf("Plantilla:\n");
    printf("  Vertices: %i\n",g->plantilla.nvertices);
    printf("  Area: %f\n",polygon_area(&(g->plantilla)));

    printf("Huecos: %i\n",g->nhuecos);
    for (i=0;i<g->nhuecos;i++)
    {
        printf("  Hueco %i:\n",i+1);
        printf("    Vertices: %i\n",g->huecos[i].nvertices);
        printf("    Area: %f\n",polygon_area(&(g->huecos[i])));
    }

    printf("Area Util: %f\n",g->area);
    printf("Volumen Util: %f\n",g->volumen);

    printf("Patrones: %i\n",g->npatrones);
    for (i=0;i<g->npatrones;i++)
    {
        printf("  Patrones %i:\n",i+1);
        printf("    Vertices: %i\n",g->patrones[i].nvertices);
        printf("    Area: %f\n",polygon_area(&(g->patrones[i])));
    }

    float oldvol;

//    oldvol=f_volumen(&(g->plantilla),g->nhuecos,g->huecos,0,NULL,sqrt(DELTA));

//   printf("Volumen calculado a la antigua: %f\n",oldvol);

}


float f_volumen(  polygon *plantilla,
                  int nhuecos,
                  polygon *huecos,
                  int npatrones,
                  polygon *patrones,
                  float delta)
{
    float vol=0;
    float minx, miny, maxx, maxy;

    point p;
    int i;

    polygon_minbox(plantilla, &minx, &miny, &maxx, &maxy);

    for (p.x=minx;p.x<maxx;p.x+=delta)
    {
        for (p.y=miny;p.y<maxy;p.y+=delta)
        {
            if (polygon_pointin(plantilla, &p))
            {
                int valido=1;

                for (i=0;i<nhuecos && valido==1;i++)
                {
                    if (polygon_pointin(&huecos[i], &p))
                    {
                        valido=0;
                    }
                }

                for (i=0;i<npatrones && valido==1;i++)
                {
                    if (polygon_pointin(&patrones[i], &p))
                    {
                        valido=0;
                    }
                }

                if (valido)
                {
                    float min=distance_pointpolygon(&p, plantilla, NULL);

                    for (i=0;i<nhuecos;i++)
                    {
                        float min2=distance_pointpolygon(&p,&huecos[i],NULL);
                        if (min2<min)
                        {
                            min=min2;
                        }
                    }

                    for (i=0;i<npatrones;i++)
                    {
                        float min2=distance_pointpolygon(&p, &patrones[i],NULL);
                        if (min2<min)
                        {
                            min=min2;
                        }
                    }
                    /*
                    acquire_screen();
                    putpixel(screen,(int)p.x,(int)p.y,makecol((int)min,(int)min,(int)min));
                    release_screen();
                    */
                    vol+=min*delta*delta;
                }
            }
        }
    }
    return vol;
}
