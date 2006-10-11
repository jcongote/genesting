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


genesting* leer_archivo(char *arc_name);

void genesting_init(genesting *g);

void genesting_createpoblation();

void genesting_newgeneration();

#endif
