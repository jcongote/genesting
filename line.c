
#include <math.h>

#include <line.h>

int line_LineIntersection(line *l1, line *l2)
{
    float Ax,Ay,Bx,By,Cx,Cy,Dx,Dy;
    float distAB, theCos, theSin, newX, ABpos;

    Ax = l1->v1.x;
    Ay = l1->v1.y;
    Bx = l1->v2.x;
    By = l1->v2.y;
    Cx = l2->v1.x;
    Cy = l2->v1.y;
    Dx = l2->v2.x;
    Dy = l2->v2.y;

    if (Ax==Bx && Ay==By || Cx==Dx && Cy==Dy)
        return 0;

    Bx-=Ax;
    By-=Ay;
    Cx-=Ax;
    Cy-=Ay;
    Dx-=Ax;
    Dy-=Ay;

    distAB=sqrt(powf(Bx,2)+powf(By,2));

    theCos=Bx/distAB;
    theSin=By/distAB;

    newX=Cx*theCos+Cy*theSin;
    Cy  =Cy*theCos-Cx*theSin;
    Cx=newX;

    newX=Dx*theCos+Dy*theSin;
    Dy  =Dy*theCos-Dx*theSin;
    Dx=newX;

    if (Cy==Dy)
        return 0;

    ABpos=Dx+(Cx-Dx)*Dy/(Dy-Cy);

    return (distAB<=ABpos ? 1 :0);
}
