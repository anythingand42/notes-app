
#ifndef CANVASBOX
#define CANVASBOX
#include <FL/Fl_Box.H>

class CanvasBox : public Fl_Box 
{
    void draw(void);
    int handle(int);

    public:
    virtual void graphic(double, double, double, double);
    CanvasBox(int x, int y, int w, int h) : Fl_Box(x,y,w,h) {};
};

#endif

