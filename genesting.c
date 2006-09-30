/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <allegro.h>

#define line ge_line
#define polygon ge_polygon
#include "polygon.h"
#undef line
#undef polygon


#define DELTA 3.0

struct _genesting
{
    ge_polygon plantilla;
    ge_polygon *huecos;
    ge_polygon *patrones;
    unsigned int nhuecos;
    unsigned int npatrones;
    float area;
    float volumen;
};

typedef struct _genesting genesting;

struct _posicion
{
    float x;
    float y;
    float t;
    unsigned int id;
};

typedef struct _posicion posicion;

struct _individuo
{
    genesting *ambiente;

    posicion *posgen;

    unsigned int ngenes;
    float fitness;
    float areautil;
};

typedef struct _individuo individuo;

genesting* leer_archivo(char *arc_name);
void show(genesting *g);
void draw_plantilla_huecos(BITMAP *bb,genesting *g);
void draw_patrones(BITMAP *bb,int npatrones, ge_polygon *patrones);
void init_allegro();
float f_volumen(  ge_polygon *plantilla,
                  int nhuecos,
                  ge_polygon *huecos,
                  int npatrones,
                  ge_polygon *patrones,
                  float delta);
float fitness(individuo *i);


int main(int argc, char **argv)
{
    genesting *g;

    init_allegro();

    if (argc<2)
    {
        fprintf(stderr,"Se deben entrar ingresar el nombre del archivo a leer\n");
        return 1;
    }

    g=leer_archivo(argv[1]);

    show(g);

    individuo pepe;

    pepe.ambiente = g;
    pepe.ngenes = 2;
    pepe.posgen = (posicion*) malloc(sizeof(posicion)*2);

    pepe.posgen[0].x=-210;
    pepe.posgen[0].y=-200;
    pepe.posgen[0].t=2;
    pepe.posgen[0].id=0;

    pepe.posgen[1].x=-200;
    pepe.posgen[1].y=-200;
    pepe.posgen[1].t=0;
    pepe.posgen[1].id=1;


    int movimiento=1;
    float anterior,nuevo;

    do
    {
        int i;
        clear_bitmap(screen);
        draw_plantilla_huecos(screen,g);
        anterior=fitness(&pepe);
        movimiento++;
        movimiento%=6;

        for (i=0;i<pepe.ngenes;i++)
        {

            switch(movimiento)
            {
                case 0:
                pepe.posgen[i].x+=2;
                nuevo=fitness(&pepe);
                if (nuevo>anterior)
                {
                    pepe.posgen[i].x-=2;
                    anterior=nuevo;
                }
                break;
                case 1:

                pepe.posgen[i].x-=2;
                nuevo=fitness(&pepe);
                if (nuevo>anterior)
                {
                    pepe.posgen[i].x+=2;
                    anterior=nuevo;
                }
                break;
                case 2:
                pepe.posgen[i].y+=2;
                nuevo=fitness(&pepe);
                if (nuevo>anterior)
                {
                    pepe.posgen[i].y-=2;
                    anterior=nuevo;
                }
                break;
                case 3:
                pepe.posgen[i].y-=2;
                nuevo=fitness(&pepe);
                if (nuevo>anterior)
                {
                    pepe.posgen[i].y+=2;
                    anterior=nuevo;
                }
                case 4:

                pepe.posgen[i].t-=0.1;
                nuevo=fitness(&pepe);
                if (nuevo>anterior)
                {
                    pepe.posgen[i].t+=0.1;
                    anterior=nuevo;
                }

                break;
                case 5:

                pepe.posgen[i].t+=0.1;
                nuevo=fitness(&pepe);
                if (nuevo>anterior)
                {
                    pepe.posgen[i].t-=0.1;
                    anterior=nuevo;
                }
                break;
            }
        }
        printf("Fitnes de pepe: %f  -- [%f,%f,%f]\n",anterior,pepe.posgen[0].x,pepe.posgen[0].y,pepe.posgen[0].t);
    }
    while(!key[KEY_ESC]);


    return 0;
}
END_OF_MAIN();

genesting* leer_archivo(char *arc_name)
{
    FILE* arc;
    genesting *g;
    int npoly;
    int i,j;

    g =(genesting*) malloc (sizeof(genesting));

    arc=fopen(arc_name,"r");

    fscanf(arc,"%i",&npoly);

    g->nhuecos =0;
    g->npatrones =0;

    g->huecos=(ge_polygon*) malloc(sizeof(ge_polygon)*npoly-1);
    g->patrones=(ge_polygon*) malloc(sizeof(ge_polygon)*npoly-1);

    for (i=0;i<npoly;i++)
    {
        int nvert,tipo;

        ge_polygon *p;
        fscanf(arc,"%i %i",&nvert,&tipo);
        switch(tipo)
        {
            case 1:
            p=&(g->plantilla);
            break;
            case 2:
            p=&(g->patrones[g->npatrones++]);
            break;
            case 3:
            p=&(g->huecos[g->nhuecos++]);
            break;
        }
        p->nvertices = nvert;
        p->v=(point*) malloc(sizeof(point)*nvert);
        for (j=0;j<nvert;j++)
        {
            fscanf(arc,"%f %f",&(p->v[j].x),&(p->v[j].y));
        }
    }
    g->huecos=(ge_polygon*) realloc(g->huecos,sizeof(ge_polygon)*g->nhuecos);
    g->patrones=(ge_polygon*) realloc(g->patrones,sizeof(ge_polygon)*g->npatrones);

    fclose(arc);

    float minx,miny;
    float maxx,maxy;

    g->area = polygon_Area(&(g->plantilla));

    minbox_Polygon(&(g->plantilla), &minx, &miny, &maxx, &maxy);

    translate_Polygon(&(g->plantilla), minx, miny);

    for (i=0;i<g->nhuecos;i++)
    {
        translate_Polygon(&(g->huecos[i]), minx, miny);
        g->area -= polygon_Area(&(g->huecos[i]));
    }

    for (i=0;i<g->npatrones;i++)
    {
        minbox_Polygon(&(g->patrones[i]), &minx, &miny, &maxx, &maxy);
        translate_Polygon(&(g->patrones[i]), minx, miny);
    }

    g->volumen = f_volumen (&(g->plantilla),g->nhuecos,g->huecos,0,NULL,DELTA);

    return g;
};

void show(genesting *g)
{
    int i;

    printf("Mostrando informacion de genesting\n");
    printf("Plantilla:\n");
    printf("  Vertices: %i\n",g->plantilla.nvertices);
    printf("  Area: %f\n",polygon_Area(&(g->plantilla)));

    printf("Huecos: %i\n",g->nhuecos);
    for (i=0;i<g->nhuecos;i++)
    {
        printf("  Hueco %i:\n",i+1);
        printf("    Vertices: %i\n",g->huecos[i].nvertices);
        printf("    Area: %f\n",polygon_Area(&(g->huecos[i])));
    }

    printf("Area Util: %f\n",g->area);
    printf("Volumen Util: %f\n",g->volumen);


    printf("Patrones: %i\n",g->npatrones);
    for (i=0;i<g->npatrones;i++)
    {
        printf("  Patrones %i:\n",i+1);
        printf("    Vertices: %i\n",g->patrones[i].nvertices);
        printf("    Area: %f\n",polygon_Area(&(g->patrones[i])));
    }
}

void draw_plantilla_huecos(BITMAP *bb,genesting *g)
{

    int i,j;

    int a,b,c,d;
    acquire_bitmap(bb);

    //Dibujar Plantilla
    for (i=0;i<g->plantilla.nvertices;i++)
    {
        a=(int)g->plantilla.v[i].x;
        b=(int)g->plantilla.v[i].y;
        c=(int)g->plantilla.v[(i+1)%g->plantilla.nvertices].x;
        d=(int)g->plantilla.v[(i+1)%g->plantilla.nvertices].y;
        line(bb,a,b,c,d,makecol(255,0,0));
    }

    //Dibujar Huecos
    for (i=0;i<g->nhuecos;i++)
    {
        for (j=0;j<g->huecos[i].nvertices;j++)
        {
            a=(int)g->huecos[i].v[j].x;
            b=(int)g->huecos[i].v[j].y;
            c=(int)g->huecos[i].v[(j+1)%g->huecos[i].nvertices].x;
            d=(int)g->huecos[i].v[(j+1)%g->huecos[i].nvertices].y;
            line(bb,a,b,c,d,makecol(0,0,255));
        }
    }
    release_bitmap(bb);
}

void draw_patrones(BITMAP *bb,int npatrones, ge_polygon *patrones)
{
    int i,j;

    int a,b,c,d;
    acquire_bitmap(bb);

    //Dibujar Huecos
    for (i=0;i<npatrones;i++)
    {
        for (j=0;j<patrones[i].nvertices;j++)
        {
            a=(int)patrones[i].v[j].x;
            b=(int)patrones[i].v[j].y;
            c=(int)patrones[i].v[(j+1)%patrones[i].nvertices].x;
            d=(int)patrones[i].v[(j+1)%patrones[i].nvertices].y;
            line(bb,a,b,c,d,makecol(0,255,0));
        }
    }
    release_bitmap(bb);
}

void init_allegro()
{
    int depth, res;
    allegro_init();
    depth = desktop_color_depth();
    if (depth == 0)
        depth = 32;
    set_color_depth(depth);
    res = set_gfx_mode(GFX_AUTODETECT_WINDOWED, 640, 480, 0, 0);
    if (res != 0)
    {
        allegro_message(allegro_error);
        exit(-1);
    }

    install_timer();
    install_keyboard();
    install_mouse();
    /* add other initializations here */
}

float f_volumen(  ge_polygon *plantilla,
                  int nhuecos,
                  ge_polygon *huecos,
                  int npatrones,
                  ge_polygon *patrones,
                  float delta)
{
    float vol=0;
    float minx, miny, maxx, maxy;

    point p;
    int i;

    minbox_Polygon(plantilla, &minx, &miny, &maxx, &maxy);

    for (p.x=minx;p.x<maxx;p.x+=delta)
    {
        for (p.y=miny;p.y<maxy;p.y+=delta)
        {
            if (polygon_pointinPolygon(plantilla, &p))
            {
                int valido=1;

                for (i=0;i<nhuecos && valido==1;i++)
                {
                    if (polygon_pointinPolygon(&huecos[i], &p))
                    {
                        valido=0;
                    }
                }

                for (i=0;i<npatrones && valido==1;i++)
                {
                    if (polygon_pointinPolygon(&patrones[i], &p))
                    {
                        valido=0;
                    }
                }

                if (valido)
                {
                    float min=distance_pointPolygon(&p, plantilla);

                    for (i=0;i<nhuecos;i++)
                    {
                        float min2=distance_pointPolygon(&p, &huecos[i]);
                        if (min2<min)
                        {
                            min=min2;
                        }
                    }

                    for (i=0;i<npatrones;i++)
                    {
                        float min2=distance_pointPolygon(&p, &patrones[i]);
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


float fitness(individuo *ind) //Tiene que estar mala!!!! hay que provarla
{
    //El fitnes es... el porcentaje de volumen generado sobre volumen posible

    int i,j;

    float volcalc;

    ge_polygon *temp;

    temp = (ge_polygon*) malloc(sizeof(ge_polygon)*ind->ngenes);

    for (i=0;i<ind->ngenes;i++)
    {
        temp[i].nvertices = ind->ambiente->patrones[ind->posgen[i].id].nvertices;
        temp[i].v=(point*) malloc(sizeof(point)*temp[i].nvertices);

        for (j=0;j<temp[i].nvertices;j++)
        {
            temp[i].v[j].x=ind->ambiente->patrones[ind->posgen[i].id].v[j].x;
            temp[i].v[j].y=ind->ambiente->patrones[ind->posgen[i].id].v[j].y;
        }

        rotate_Polygon(&(temp[i]),ind->posgen[i].t);
        translate_Polygon(&(temp[i]),ind->posgen[i].x, ind->posgen[i].y);
    }

    draw_patrones(screen,ind->ngenes,temp);

    volcalc = f_volumen (&(ind->ambiente->plantilla),ind->ambiente->nhuecos,ind->ambiente->huecos,ind->ngenes,temp,DELTA);

    for (i=0;i<ind->ngenes;i++)
    {
        free(temp[i].v);
    }
    free (temp);

    ind->fitness = volcalc/(ind->ambiente->volumen);
    //return (ind->fitness);
    return volcalc;
};



/*
    integrar la figura entre x1,y1 y x2,y2 teniendo x1,y1,x2,y2:

    x1: menor valor de x de la plantilla

    y1: menor valor de y de la plantilla

    x2: mayor valor de x de la plantilla

    y2: mayor valor de y de la plantilla


    ubicar el valor de la funcion en los puntos:
    (x1,y1), (x1,y2), (x2,y1), (x2,y2)

    Si alguno de estos puntos no pertenece a la plantilla o esta en un hueco
    entonces subdividir el area en cuatro rectangulos de esta forma:

    1) (x1,y1) (x1+x2/2,y1+y2/2)
    2) (x1+x2/2,y1) (x2,y1+y2/2)
    3) (x1,y1+y2/2) (x1+x2/2,y2)
    4) (x1+x2/2,y1+y2/2) (x2,y2)

    si la suma de el area de estos cuadros es igual a la suma del area de cada
    uno de los subcuadros mas una tolerancia, entonces parar y el area es igual
    a la suma de los subcuadros sino subdividir.

*/

/*
Encontrando facilmente el medial axe, o la division de voronoi
si se tiene el area de cada uno de las celdas de voronoi y se
multiplica por la maxima altura
entonces se puede encontrar el volumen... dado que la figura
es un cono.

*/




