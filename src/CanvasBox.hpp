#ifndef CANVASBOX
#define CANVASBOX

#include <FL/Fl_Box.H>

#include "OffscreenDrawer.hpp"

class CanvasBox : public Fl_Box 
{
    OffscreenDrawer offscreen_drawer;

    void draw();
    int handle(int);

public:
    CanvasBox(int, int, int, int);
    uchar* GetRGBData();
    int GetRGBDataSize();
    void DrawRGBImage(uchar*);
};

#endif
