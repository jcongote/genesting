/*!\file graphics.h*/

#ifndef FILE_GRAPHICS_H
#define FILE_GRAPHICS_H

#define graphics 1

#define DELTA 1

#if graphics
void init_graphics();
void draw_rect(float minx, float miny, float maxx, float maxy,int r, int g, int b);
void draw_line(float x1, float y1, float x2, float y2);
void getscreen();
void relscreen();
void clearscreen();
#endif

#endif
