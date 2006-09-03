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
#include <math.h>
#include <stdio.h>

typedef struct _point{
	float x,y;
} point;

typedef struct _polygon{
	unsigned int nvertices;
	point *v;
} polygon;

typedef struct _line{
	point v1,v2;
} line;

float polygon_Area(polygon *p){
	unsigned int i,j,n;
	float area = 0;
	n = p->nvertices;

	for (i=0;i<n;i++) {
		j = (i + 1) % n;
		area += p->v[i].x * p->v[j].y;
		area -= p->v[i].y * p->v[j].x;
	}
	area /= 2;
	
	return(area < 0 ? -area : area);	
}

int polygon_PointinPolygon(polygon *p, point *f){
	int c=0;
	unsigned int i,j,n=0;
	
	for (i=0,j=n-1;i<p->nvertices;j=i++){
        if ((((p->v[i].y<=f->y) && (f->y<p->v[j].y)) ||
             ((p->v[j].y<=f->y) && (f->y<p->v[i].y))) &&
            (f->x < (p->v[j].x - p->v[i].x) * (f->y - p->v[i].y) / (p->v[j].y - p->v[i].y) + p->v[i].x))
			c = !c;
	}
	return c;
}

int line_LineIntersection(line *l1, line *l2){
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
	
	if (Ax==Bx && Ay==By || Cx==Dx && Cy==Dy) return 0;
//  Fail if either line is undefined.
  

//  (1) Translate the system so that point A is on the origin.
	Bx-=Ax; By-=Ay;
	Cx-=Ax; Cy-=Ay;
 	Dx-=Ax; Dy-=Ay;

//  Discover the length of segment A-B.
	distAB=sqrt(powf(Bx,2)+powf(By,2));

//  (2) Rotate the system so that point B is on the positive X axis.
	theCos=Bx/distAB;
	theSin=By/distAB;
	newX=Cx*theCos+Cy*theSin;
	Cy  =Cy*theCos-Cx*theSin; Cx=newX;
	newX=Dx*theCos+Dy*theSin;
	Dy  =Dy*theCos-Dx*theSin; Dx=newX;

//  Fail if the lines are parallel.
	if (Cy==Dy) return 0;

//  (3) Discover the position of the intersection point along line A-B.
 	ABpos=Dx+(Cx-Dx)*Dy/(Dy-Cy);

//  (4) Apply the discovered position to line A-B in the original coordinate system.
//	*X=Ax+ABpos*theCos;
//	*Y=Ay+ABpos*theSin;

	return (distAB<=ABpos ? 1 :0);	
}

int polygon_Overlapping(polygon *p, polygon *q){

}


int main(int argc, char **argv){

	return 0;
}
