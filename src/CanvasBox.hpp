#ifndef CANVASBOX
#define CANVASBOX

#include <FL/Fl_Box.H>
#include <FL/fl_draw.H>
#include "Drawer.hpp"

class CanvasBox : public Fl_Box 
{
    Drawer drawer;

    void draw();
    int handle(int);

public:
    CanvasBox(int, int, int, int);
};

#endif

