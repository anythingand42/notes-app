#include <FL/Enumerations.H>
#include <stdio.h>

#include <FL/Fl.H>
#include <FL/fl_draw.H>

#include "CanvasBox.hpp"
#include "Drawer.hpp"

CanvasBox::CanvasBox(int x, int y, int w, int h) : Fl_Box(x, y, w, h) {}

void CanvasBox::draw(void)
{
    if (!drawer.GetOffscreenBuf()) {
        drawer.InitOffscreenBuf();
    }
    fl_copy_offscreen(x(), y(), w(), h(), drawer.GetOffscreenBuf(), x(), y());
}

int CanvasBox::handle(int event)
{
    switch(event) {
        case FL_PUSH:
            // printf("PUSH x:%i, y:%i\n", Fl::event_x(), Fl::event_y());
            if (Fl::event_button() == FL_LEFT_MOUSE) {
                drawer.HandlePathStart();
            } else if (Fl::event_button() == FL_RIGHT_MOUSE) {
                drawer.HandleErase();
                redraw();
            }
            return 1;
        case FL_DRAG: {
            int t = Fl::event_inside(this);
            if (t) {
                // printf("DRAG inside x:%i, y:%i\n", Fl::event_x(), Fl::event_y());
                if (Fl::event_button() == FL_LEFT_MOUSE) {
                    drawer.HandlePathDraw();
                } else if (Fl::event_button() == FL_RIGHT_MOUSE) {
                    drawer.HandleErase();
                }
                redraw();
            } else {
                printf("DRAG outside x:%i, y:%i\n", Fl::event_x(), Fl::event_y());
            }
            return 1;
        }
        case FL_RELEASE:
            // printf("RELEASE x:%i, y:%i\n", Fl::event_x(), Fl::event_y());
            if (Fl::event_button() == FL_LEFT_MOUSE) {
                drawer.HandlePathEnd();
                redraw();
            }
            return 1;
        // in order to catch FL_MOVE event
        case FL_ENTER:
            return 1;
        case FL_MOVE:
            drawer.HandleTextReset();
            return 1;
        case FL_SHORTCUT:
            drawer.HandleTextInput();
            redraw();
            return 1;
        default:
            return Fl_Widget::handle(event);
    }
}
