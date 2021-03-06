#include <FL/Fl.H>
#include <FL/Enumerations.H>
#include <FL/Fl_Box.H>

#include "CanvasBox.hpp"
#include "OffscreenDrawer.hpp"

CanvasBox::CanvasBox(int x, int y, int w, int h) : Fl_Box(x, y, w, h) {}

void CanvasBox::draw(void)
{
    if (!offscreen_drawer.GetBuf()) {
        offscreen_drawer.InitBuf();
    }
    fl_copy_offscreen(x(), y(), w(), h(), offscreen_drawer.GetBuf(), x(), y());
}

uchar* CanvasBox::GetRGBData()
{
    return offscreen_drawer.GetRGBData();
}

int CanvasBox::GetRGBDataSize()
{
    return offscreen_drawer.GetRGBDataSize();
}

void CanvasBox::DrawRGBImage(uchar* rgb_data)
{
    offscreen_drawer.DrawRGBImage(rgb_data);
    redraw();
}

int CanvasBox::handle(int event)
{
    switch(event) {
        case FL_PUSH:
            // printf("PUSH x:%i, y:%i\n", Fl::event_x(), Fl::event_y());
            if (Fl::event_button() == FL_LEFT_MOUSE) {
                offscreen_drawer.HandlePathStart();
            } else if (Fl::event_button() == FL_RIGHT_MOUSE) {
                offscreen_drawer.HandleErase();
                redraw();
            }
            return 1;
        case FL_DRAG: {
            int t = Fl::event_inside(this);
            if (t) {
                // printf("DRAG inside x:%i, y:%i\n", Fl::event_x(), Fl::event_y());
                if (Fl::event_button() == FL_LEFT_MOUSE) {
                    offscreen_drawer.HandlePathDraw();
                } else if (Fl::event_button() == FL_RIGHT_MOUSE) {
                    offscreen_drawer.HandleErase();
                }
                redraw();
            }
            return 1;
        }
        case FL_RELEASE:
            // printf("RELEASE x:%i, y:%i\n", Fl::event_x(), Fl::event_y());
            if (Fl::event_button() == FL_LEFT_MOUSE) {
                offscreen_drawer.HandlePathEnd();
                redraw();
            }
            return 1;
        // in order to catch FL_MOVE event
        case FL_ENTER:
            return 1;
        case FL_MOVE:
            offscreen_drawer.HandleTextReset();
            return 1;
        case FL_SHORTCUT:
            offscreen_drawer.HandleTextInput();
            redraw();
            return 1;
        default:
            return Fl_Widget::handle(event);
    }
}
