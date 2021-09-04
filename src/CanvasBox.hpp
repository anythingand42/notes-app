#ifndef CANVASBOX
#define CANVASBOX

#include <FL/Fl_Box.H>
#include <FL/fl_draw.H>
#include "OffscreenDrawer.hpp"

class CanvasBox : public Fl_Box 
{
    OffscreenDrawer offscreen_drawer;

    void draw();
    int handle(int);

public:
    CanvasBox(int, int, int, int);
};

#endif

