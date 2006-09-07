
#include <point.h>

/** \struct _line
Define una linea o mas estrictamente un segmento de recta entre dos puntos
dados.
*/
struct _line
{
    point v1,v2;
};

typedef struct _line line;

int line_LineIntersection(line *l1, line *l2);
