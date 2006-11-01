/*!\addtogroup geometry
@{
*/

/*!\file polygon_holes.c
Este archivo define las funciones que se utilizan en el Objeto Poligono con
huecos
*/
#include <float.h>
#include <math.h>

#include "geometry.h"


struct _polygonholes
{
    unsigned int nh;
    polygon p;
    polygon *h;

    float area;
    float volumen;
};


/*!\fn float polygonholes_area(polygonholes p)
La funcion calcula el area de un Poligono con huecos, para hacer esto
calculamos el area del poligono formado por el borde exterior y le restamos
el area de cada uno de los huecos.

\param [in] p Poligono simple en 2 dimensiones
\return Area del poligono simple con huecos
*/
float polygonholes_area(polygonholes p)
{
    if (!isnan(p->area))
        return p->area;

    unsigned int i;
    p->area = polygon_area(p->p);
    for (i=0;i<p->nh;i++)
    {
        p->area -= polygon_area(p->h[i]);
    }
    return p->area;
}

/*!\fn bool polygonholes_pointinhole(polygonholes p, point f)
Identifica si un punto esta dentro de un hueco del poligono.

\param [in] p Poligono Simple con huecos
\param [in] f Punto en 2 dimensiones
\return Verdadero si el punto esta dentro de uno de los huecos, falso en
caso contrario
*/

static bool polygonholes_pointinhole(polygonholes p, point f)
{
    unsigned int i;

    for (i=0;i<p->nh;i++)
    {
        if (polygon_pointin(p->h[i], f))
        {
            return true;
        }
    }
    return false;
}



/*!\fn bool polygonholes_pointin(polygon_holes p, point f)
Esta funcion identifica si un punto esta dentro de un poligono con huecos
por lo tanto evalua inicialmente que el punto este dentro del poligono
exterior y posterioremente que no se encuentre dentro de los huecos

\param [in] p Poligono simple con huecos en 2 dimensiones
\param [in] f Punto en 2 dimensiones
\return Verdadero en caso que el punto este dentro del poligono, falso en
        caso contrario
*/
bool polygonholes_pointin(polygonholes p, point f)
{
    if (polygon_pointin(p->p, f))
    {
        return !polygonholes_pointinhole(p, f);
    }
    return false;
}

/*!\fn bool polygonholes_polygonin(polygonholes p, polygon q)
La funcion evalua que el poligono q esta completamente adentro del poligono p
entonces todos los puntos de q deben estar dentro de p y no se deben intersectar
las lineas que los conforman

\param [in] p Poligono Simple con huecos
\param [in] q Poligono Simple
\return Verdadero en caso del que poligono q este dentro del poligono p, falso
        en caso contrario
*/
bool polygonholes_polygonin(polygonholes p, polygon q)
{
    if (polygon_polygonin(p->p,q))
    {
        unsigned int i;
        for (i=0;i<p->nh;i++)
        {
            if (polygon_overlapping(p->h[i], q))
            {
                return false;
            }
        }
        return true;
    }
    return false;
}

/*!\fn float polygonholes_volumen(polygonholes p)
Calcula el volumen generado por el poligono con huecos, definiendo asi el
poligono como una figura tridimensional de la siguiente manera, la base del
solido esta conformada por el poligono con huecos y la altura del solido
en cada punto es igual a la distancia del punto a uno de los bordes.

El metodo para calcular el volumen se basa en encerrar el poligono en un
rectangulo que lo contenga, y sucesivamente partir el poligono en dos
subrectangulos respecto al lado mas largo calculando el volumen de cada
uno de los subrectangulos y sumando el valor y esto se hace de manera recursiva.

    - Cuando el subrectangulo esta completamente fuera del poligono no
      calculamos el volumen y regresamos un valor de 0

    - Cuando el subrectangulo tiene un area muy pequena definida por un da
      se calcula el centro del subrectangulo y se verifica que este dentro
      del poligono, si este es el caso, entonces se devuelve el valor del
      area multiplicada por la distancia del punto medio al borde, si el
      punto medio es 0 se devuelve 0.

    - Cuando el subrectangulo esta dentro del poligono y los vertices de este
      tienen su menor distancia respecto a un mismo segmento de recta se
      calcula el volumen del poliedro formado como el promedio de las distancias
      de los vertices multiplicados por el area del subrectangulo.

    - Cuando el subrectangulo esta dentro del poligono y los vertices de este
      tienen su menor distancia respecto a un mismo punto, que se da en el
      caso de los poligonos concavos, entonces se calcula el volumen como el
      volumen formado por la interseccion entre el cono que genera el punto y
      el subrectangulo, la formula que da este volumen es:
      \f$\int_{a}^{b}\int_{c}^{d}\sqrt{((x-h)^2 + (y-k)^2)} dx dy\f$ donde
      \f$(h,k)\f$ son las coordenadas del punto con respecto al que se toma la
      distancia, \f$(a,b)\f$ son los valores en que se desplaza \f$y\f$,
      \f$(c,d)\f$ son los valores en que se desplaza \f$x\f$.

Esta funcion evalua menos puntos si en ves de dividir el rectangulo en 2 segun
su lado mas largo, se divide en 4 partes iguales, pero esta aproximacion puede
tener problemas cuando la proporcion del los lados del subrectangulo es muy
grande.

\param [in] p Poligono simple con huecos en 2 dimensiones
\return Volumen generado
*/

float polygonholes_volumen(polygonholes p)
{
    return polygonholes_volumen_box(p,b);
}

static float volumen_prisma(float a,
                            float b,
                            float c,
                            float d,
                            float da)
{
    return ((a+b+c+d)/4.0 * da);
}


/*!\bug
Esta funcion para calcular el volumen de un rectangulo que intersecta un
cono tiene valores indeterminados en algunos segmentos del problema, es
posible que este error aparesca solo en algunos casos, pero hay que examinar
mas la funcion.
*/
static float volumen_conorectangulo( float a,
                                     float b,
                                     float c,
                                     float d,
                                     float h,
                                     float k)
{
    float v;
    v = (2.0*a*c*sqrt(pow(-c + h,2.0) + pow(a - k,2.0))
         - 2.0*a*h*sqrt(pow(-c + h,2.0) + pow(a - k,2.0))
         - 2.0*a*d*sqrt(pow(-d + h,2.0) + pow(a - k,2.0))
         + 2.0*a*h*sqrt(pow(-d + h,2.0) + pow(a - k,2.0))
         - 2.0*b*c*sqrt(pow(-c + h,2.0) + pow(b - k,2.0))
         + 2.0*b*h*sqrt(pow(-c + h,2.0) + pow(b - k,2.0))
         + 2.0*b*d*sqrt(pow(-d + h,2.0) + pow(b - k,2.0))
         - 2.0*b*h*sqrt(pow(-d + h,2.0) + pow(b - k,2.0))
         - 2.0*c*k*sqrt(pow(-c + h,2.0) + pow(a - k,2.0))
         + 2.0*h*k*sqrt(pow(-c + h,2.0) + pow(a - k,2.0))
         + 2.0*d*k*sqrt(pow(-d + h,2.0) + pow(a - k,2.0))
         - 2.0*h*k*sqrt(pow(-d + h,2.0) + pow(a - k,2.0))
         + 2.0*c*k*sqrt(pow(-c + h,2.0) + pow(b - k,2.0))
         - 2.0*h*k*sqrt(pow(-c + h,2.0) + pow(b - k,2.0))
         - 2.0*d*k*sqrt(pow(-d + h,2.0) + pow(b - k,2.0))
         + 2.0*h*k*sqrt(pow(-d + h,2.0) + pow(b - k,2.0))
         - pow(a,3.0)*log(-c + h + sqrt(pow(-c + h,2.0) + pow(a - k,2.0)))
         + 3.0*pow(a,2.0)*k*log(-c + h + sqrt(pow(-c + h,2.0) + pow(a - k,2.0)))
         - 3.0*a*pow(k,2.0)*log(-c + h + sqrt(pow(-c + h,2.0) + pow(a - k,2.0)))
         + pow(k,3.0)*log(-c + h + sqrt(pow(-c + h,2.0) + pow(a - k,2.0)))
         + pow(a,3.0)*log(-d + h + sqrt(pow(-d + h,2.0) + pow(a - k,2.0)))
         - 3.0*pow(a,2.0)*k*log(-d + h + sqrt(pow(-d + h,2.0) + pow(a - k,2.0)))
         + 3.0*a*pow(k,2.0)*log(-d + h + sqrt(pow(-d + h,2.0) + pow(a - k,2.0)))
         - pow(k,3.0)*log(-d + h + sqrt(pow(-d + h,2.0) + pow(a - k,2.0)))
         + pow(b,3.0)*log(-c + h + sqrt(pow(-c + h,2.0) + pow(b - k,2.0)))
         - 3.0*pow(b,2.0)*k*log(-c + h + sqrt(pow(-c + h,2.0) + pow(b - k,2.0)))
         + 3.0*b*pow(k,2.0)*log(-c + h + sqrt(pow(-c + h,2.0) + pow(b - k,2.0)))
         - pow(k,3.0)*log(-c + h + sqrt(pow(-c + h,2.0) + pow(b - k,2.0)))
         - pow(b,3.0)*log(-d + h + sqrt(pow(-d + h,2.0) + pow(b - k,2.0)))
         + 3.0*pow(b,2.0)*k*log(-d + h + sqrt(pow(-d + h,2.0) + pow(b - k,2.0)))
         - 3.0*b*pow(k,2.0)*log(-d + h + sqrt(pow(-d + h,2.0) + pow(b - k,2.0)))
         + pow(k,3.0)*log(-d + h + sqrt(pow(-d + h,2.0) + pow(b - k,2.0)))
         + pow(c,3.0)*log(a + sqrt(pow(-c + h,2.0) + pow(a - k,2.0)) - k)
         - 3.0*pow(c,2.0)*h*log(a + sqrt(pow(-c + h,2.0) + pow(a - k,2.0)) - k)
         + 3.0*c*pow(h,2.0)*log(a + sqrt(pow(-c + h,2.0) + pow(a - k,2.0)) - k)
         - pow(h,3.0)*log(a + sqrt(pow(-c + h,2.0) + pow(a - k,2.0)) - k)
         - pow(d,3.0)*log(a + sqrt(pow(-d + h,2.0) + pow(a - k,2.0)) - k)
         + 3.0*pow(d,2.0)*h*log(a + sqrt(pow(-d + h,2.0) + pow(a - k,2.0)) - k)
         - 3.0*d*pow(h,2.0)*log(a + sqrt(pow(-d + h,2.0) + pow(a - k,2.0)) - k)
         + pow(h,3.0)*log(a + sqrt(pow(-d + h,2.0) + pow(a - k,2.0)) - k)
         - pow(c,3.0)*log(b + sqrt(pow(-c + h,2.0) + pow(b - k,2.0)) - k)
         + 3.0*pow(c,2.0)*h*log(b + sqrt(pow(-c + h,2.0) + pow(b - k,2.0)) - k)
         - 3.0*c*pow(h,2.0)*log(b + sqrt(pow(-c + h,2.0) + pow(b - k,2.0)) - k)
         + pow(h,3.0)*log(b + sqrt(pow(-c + h,2.0) + pow(b - k,2.0)) - k)
         + pow(d,3.0)*log(b + sqrt(pow(-d + h,2.0) + pow(b - k,2.0)) - k)
         - 3.0*pow(d,2.0)*h*log(b + sqrt(pow(-d + h,2.0) + pow(b - k,2.0)) - k)
         + 3.0*d*pow(h,2.0)*log(b + sqrt(pow(-d + h,2.0) + pow(b - k,2.0)) - k)
         - pow(h,3.0)*log(b + sqrt(pow(-d + h,2.0) + pow(b - k,2.0)) - k))/6.0;

    if (isnan(v)){
            return (b-a)*(d-c)*sqrt(h-(((b-a)/2.0)+a),k-(((d-c)/2.0)+c));
    }
    return v;
}


float polygonholes_volumen_box( polygonholes p,box b)
{
    if (polygonholes_boxin(p,b)){
        if (box_area(b)<da){
            if ()// centro dentro del pol
            {

            }
            else
            {
                    return 0;
            }
        }

        ref = box_polygonholesref(b,p);
        if (ref != NULL){
            if () // ref is point

            if () // ref is line

        } else  {
            //dividir box y calcular recursivamente
        }
    }
    return 0;

    if (!polygon_overlapping(p->p, &rec)) //OK
    {
        vol = 0;
    }
    else if (da < DELTA) // OK
    {
        point pm;

        pm.x=minx+((maxx-minx)/2.0);
        pm.y=miny+((maxy-miny)/2.0);

        if (polygonholes_pointin(p, &pm))
        {
            vol = da * distance_pointpolygonholes(&pm, p,NULL);
        }
        else
        {
            vol = 0;
        }
    }
    else
    {

        int i;
        float dis[4];
        line ref[4];
        for (i=0;i<4;i++)
        {
            dis[i]=distance_pointpolygonholes(&q[i], p,&ref[i]);
        }

        if( polygonholes_polygonin(p, &rec)
                &&
                (line_equal(&ref[0],&ref[1]) &&
                 line_equal(&ref[1],&ref[2]) &&
                 line_equal(&ref[2],&ref[3]))

          ) // OK
        {
            if (line_ispoint(&ref[0]))// Si es un punto entonces se utiliza la formula del cono
            {
                double a,b,c,d,h,k;
                c=minx;
                d=maxx;
                a=miny;
                b=maxy;
                h=ref[0].v1.x;
                k=ref[0].v1.y;


            }
            else // Si es una linea se utiliza la formula de la cuna
            {}
        }
        else
        {
            if (!(
                        polygonholes_pointinhole(p, &q[0]) &&
                        polygonholes_pointinhole(p, &q[1]) &&
                        polygonholes_pointinhole(p, &q[2]) &&
                        polygonholes_pointinhole(p, &q[3])
                    )) // el poligono esta dentro de p pero no en un hueco
            {
                //Opcion de corte 1
                /*
                                    vol+= polygonholes_volumen_box(p, minx, miny, (minx+maxx)/2.0, (miny+maxy)/2.0);
                                    vol+= polygonholes_volumen_box(p, (minx+maxx)/2.0, miny, maxx, (miny+maxy)/2.0);
                                    vol+= polygonholes_volumen_box(p, minx, (miny+maxy)/2.0, (minx+maxx)/2.0, maxy);
                                    vol+= polygonholes_volumen_box(p, (minx+maxx)/2.0, (miny+maxy)/2.0, maxx, maxy);
                */

                if (maxx-minx > maxy-miny)
                {
                    vol+= polygonholes_volumen_box(p, minx, miny, (minx+maxx)/2.0, maxy);
                    vol+= polygonholes_volumen_box(p, (minx+maxx)/2.0, miny, maxx, maxy);
                }
                else
                {
                    vol+= polygonholes_volumen_box(p, minx, miny, maxx, (miny+maxy)/2.0);
                    vol+= polygonholes_volumen_box(p, minx, (miny+maxy)/2.0, maxx, maxy);
                }
            }
        }
    }

    return vol;
}



/*!@}*/
