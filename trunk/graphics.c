#include <allegro.h>

#define graphics 0

void init_graphics()
{
#if graphics
    int depth, res;
    allegro_init();
    depth = desktop_color_depth();
    if (depth == 0)
        depth = 32;
    set_color_depth(depth);
    res = set_gfx_mode(GFX_AUTODETECT_WINDOWED, 640, 480, 0, 0);
    if (res != 0)
    {
        allegro_message(allegro_error);
        exit(-1);
    }

    install_timer();
    //install_keyboard();
    //install_mouse();
#endif
}

void draw_rect(float minx, float miny, float maxx, float maxy,int r, int g, int b){
#if graphics
    rect(screen, (int)minx, (int)miny, (int) maxx, (int) maxy, makecol(r,g,b));
#endif
}

void draw_line(float x1, float y1, float x2, float y2){
#if graphics
    line(screen,(int)x1,(int)y1,(int)x2,(int)y2,makecol(255,0,0));
#endif
}
