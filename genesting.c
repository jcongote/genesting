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

#include "polygon.h"

struct _genesting{
    polygon plantilla;
    polygon *huecos;
    polygon *patrones;
    unsigned int nhuecos;
    unsigned int npatrones;
};

typedef struct _genesting genesting;

genesting* leer_archivo(char *arc_name){
    FILE* arc;
    genesting *g;
    int npoly;
    int i,j;

    g =(genesting*) malloc (sizeof(genesting));

    arc=fopen(arc_name,"r");

    fscanf(arc,"%i",&npoly);

    g->nhuecos =0;
    g->npatrones =0;

    g->huecos=(polygon*) malloc(sizeof(polygon)*npoly-1);
    g->patrones=(polygon*) malloc(sizeof(polygon)*npoly-1);

    for (i=0;i<npoly;i++){
            int nvert,tipo;
            polygon *p;
            fscanf(arc,"%i %i",&nvert,&tipo);
            switch(tipo){
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
            for (j=0;j<nvert;j++){
                fscanf(arc,"%f %f",&(p->v[j].x),&(p->v[j].y));
            }
    }
    g->huecos=(polygon*) realloc(g->huecos,sizeof(polygon)*g->nhuecos);
    g->patrones=(polygon*) realloc(g->patrones,sizeof(polygon)*g->npatrones);

    fclose(arc);

    return g;
};

void show(genesting *g){
    printf("Mostrando informacion de genesting\n");
    printf("Plantilla:\n");
    printf("  Vertices: %i\n",g->plantilla.nvertices);
    printf("  Area: %f\n",polygon_Area(&(g->plantilla)));
    printf("Huecos: %i\n",g->nhuecos);
    printf("Patrones: %i\n",g->npatrones);
}

int main(int argc, char **argv){

    genesting *g;

    if (argc<2){
            fprintf(stderr,"Se deben entrar ingresar el nombre del archivo a leer\n");
            return 1;
    }

    g=leer_archivo(argv[1]);

    show(g);

	return 0;
}
