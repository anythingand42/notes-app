
#ifndef CANVASBOX
#define CANVASBOX

#include <FL/Fl_Box.H>
#include <FL/fl_draw.H>

class CanvasBox : public Fl_Box 
{
    Fl_Offscreen offscreen_buf;
    void draw(void);
    int handle(int);

public:
    CanvasBox(int x, int y, int w, int h) : Fl_Box(x,y,w,h) {
        offscreen_buf = 0;
    };
};

#endif

