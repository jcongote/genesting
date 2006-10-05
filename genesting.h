#ifndef FILE_GENESTING_H
#define FILE_GENESTING_H

#include "polygon.h"

struct _genesting
{
    polygon plantilla;
    polygon *huecos;
    polygon *patrones;
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

float fitness(individuo *i);

void genesting_init(genesting *g);

#endif
