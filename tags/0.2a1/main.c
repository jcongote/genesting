//#include <allegro.h>

#include <stdio.h>

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

    init_graphics();

    if (argc<2)
    {
        fprintf(stderr,"Se deben entrar ingresar el nombre del archivo a leer\n");
        return 1;
    }

    g=leer_archivo(argv[1]);

    show(g);

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

    oldvol=f_volumen(&(g->plantilla),g->nhuecos,g->huecos,0,NULL,0.1);

    printf("Volumen calculado a la antigua: %f\n",oldvol);

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
