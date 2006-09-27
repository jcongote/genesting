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


struct _genesting
{
    ge_polygon plantilla;
    ge_polygon *huecos;
    ge_polygon *patrones;
    unsigned int nhuecos;
    unsigned int npatrones;
};

typedef struct _genesting genesting;

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

    return g;
};

void show(genesting *g)
{
    printf("Mostrando informacion de genesting\n");
    printf("Plantilla:\n");
    printf("  Vertices: %i\n",g->plantilla.nvertices);
    printf("  Area: %f\n",polygon_Area(&(g->plantilla)));
    printf("Huecos: %i\n",g->nhuecos);
    printf("Patrones: %i\n",g->npatrones);
}

void draw_plantilla_huecos(BITMAP *bb,genesting *g){

    int i,j;

    int a,b,c,d;

    acquire_screen();

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
    release_screen();

}

void init()
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

int main(int argc, char **argv)
{
    int i,j,k,l;

    genesting *g;

    if (argc<2)
    {
        fprintf(stderr,"Se deben entrar ingresar el nombre del archivo a leer\n");
        return 1;
    }

    g=leer_archivo(argv[1]);

    show(g);

    init();
    clear_to_color(screen,makecol(255,255,255));
    draw_plantilla_huecos(screen,g);

    for (i=0;i<640;i++)
    {
        acquire_screen();
        for (j=0;j<480;j++)
        {
            point po;

            po.x = i, po.y = j;

            if (polygon_pointinPolygon(&g->plantilla, &po))
            {
                int inside_hueco=1;
                for (k=0;k<g->nhuecos && inside_hueco==1;k++)
                {
                    if (polygon_pointinPolygon(&g->huecos[k], &po)){
                        inside_hueco=0;}
                }

                if (inside_hueco)
                {
                    int min=(int)distance_pointPolygon(&po, &g->plantilla);

                    for (k=0;k<g->nhuecos;k++)
                    {
                        int min2=(int)distance_pointPolygon(&po, &g->huecos[k]);
                        if (min2<min){
                                min=min2;
                        }
                    }
                    _putpixel32(screen,i,j,makecol(min,min,min));
                }
            }
        }
        release_screen();
    }

    /*
    Encontrando facilmente el medial axe, o la division de voronoi
    si se tiene el area de cada uno de las celdas de voronoi y se
    multiplica por la maxima altura
    entonces se puede encontrar el volumen... dado que la figura
    es un cono.

    */

    while (!key[KEY_ESC])
    {
        /* put your code here */
    }

    return 0;
}
END_OF_MAIN();

