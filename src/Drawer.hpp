#ifndef DRAWER
#define DRAWER

#include <FL/Fl_Box.H>
#include <FL/fl_draw.H>

class Drawer 
{
    Fl_Offscreen offscreen_buf;
    int path_last_x, path_last_y;
    int text_start_x, text_start_y;
    int text_x, text_y;

public:
    Drawer();
    void InitOffscreenBuf();

    void HandlePathStart();
    void HandlePathDraw();
    void HandlePathEnd();

    void HandleTextReset();
    void HandleTextInput();

    void HandleErase();
    Fl_Offscreen GetOffscreenBuf();
};

#endif