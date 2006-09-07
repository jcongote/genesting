
#include <point.h>
#include <line.h>

/** \struct polygono
Define un pologino como un conjunto de puntos los cuales estan realcionados
de forma secuencial para formar la figura, generando lineas con entre los
punto (n,n+1) y uniendo el ultimo punto con el primero para cerrar el poligono
*/
struct _polygon
{
    unsigned int nvertices;
    point *v;
};

typedef struct _polygon polygon;


/**\fn float polygon_Area(polygon *p)
Calcula el area de un poligono.
*/
float polygon_Area(polygon *p);

/**
Identifica si dos poligonos tienen una interseccion.

1) Revisar que todos los vertices de p esten afuera de q
2) Revisar que todos los vertices de q esten afuera de p
3) Revisar que Todo el conjunto de lineas creadas entre p y q no se intersecten
*/
int polygon_Overlapping(polygon *p, polygon *q);

int polygon_pointinPolygon(polygon *p, point *f);

float distance_pointLine(point *p, line *l);

float distance_pointPolygon(point *p, polygon *q);
