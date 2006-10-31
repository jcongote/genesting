/*!\addtogroup geometry
@{
*/

/*!\file line.h
Declaraciones del Objeto Segmento de Linea
*/

#ifndef FILE_LINE_H
#define FILE_LINE_H

#include <stdbool.h>

#include <geometry.h>

typedef struct _line* line;

point line_getpoint1(line l);

point line_getpoint2(line l);

bool line_intersection(line l1, line l2);

bool line_equal(line l1, line l2);

bool line_ispoint(line l);

float line_areaabove(line l);

int line_pointabove(line l, point p);

#endif

/*!@}*/
