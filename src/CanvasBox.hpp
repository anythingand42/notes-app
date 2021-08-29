
#ifndef CANVASBOX
#define CANVASBOX

#include <FL/Fl_Box.H>
#include <FL/fl_draw.H>

class CanvasBox : public Fl_Box 
{
    Fl_Offscreen offscreen_buf;
    int path_last_x, path_last_y;
    int text_start_x, text_start_y;
    int text_x, text_y;

    void draw();
    int handle(int);
    
    void TextResetPosition(int, int);
    void TextHandleDrawing();
    void PathDrawLine();
    void PathResetPosition(int, int);
    void EraserDraw(int, int);

public:
    CanvasBox(int x, int y, int w, int h) : Fl_Box(x,y,w,h) {
        offscreen_buf = 0;
    };
};

#endif

