#ifndef OFFSCREENDRAWER
#define OFFSCREENDRAWER

#include <FL/fl_draw.H>

class OffscreenDrawer 
{
    Fl_Offscreen offscreen_buf;
    int path_last_x, path_last_y;
    int text_start_x, text_start_y;
    int text_x, text_y;

public:
    OffscreenDrawer();
    void InitBuf();

    void HandlePathStart();
    void HandlePathDraw();
    void HandlePathEnd();

    void HandleTextReset();
    void HandleTextInput();

    void HandleErase();

    Fl_Offscreen GetBuf();
    uchar* GetRGBData();
    int GetRGBDataSize();
    void DrawRGBImage(uchar* rgb_data);
};

#endif
