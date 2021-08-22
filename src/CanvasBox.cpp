#include <stdio.h>

#include <FL/Fl.H>
#include <FL/fl_draw.H>

#include "CanvasBox.hpp"

#define BUF_PX_SIZE 2000

int prev_x, prev_y;
void draw_line(Fl_Offscreen offscreen_buf, int x, int y)
{
    {
        fl_begin_offscreen(offscreen_buf);
        fl_color(FL_BLACK);
        fl_line_style(FL_SOLID | FL_CAP_ROUND, 3);
        fl_line(prev_x, prev_y, x, y);
        fl_end_offscreen();
    }
}

void CanvasBox::draw(void)
{
    if (!offscreen_buf) {
        offscreen_buf = fl_create_offscreen(BUF_PX_SIZE, BUF_PX_SIZE);
        {
            fl_begin_offscreen(offscreen_buf);
            fl_color(FL_WHITE);
            fl_rectf(0, 0, BUF_PX_SIZE, BUF_PX_SIZE);
            fl_end_offscreen();
        }
    }
    fl_copy_offscreen(x(), y(), w(), h(), offscreen_buf, x(), y());
}

int CanvasBox::handle(int event)
{
  switch(event) {
        case FL_PUSH:
            printf("PUSH x:%i, y:%i\n", Fl::event_x(), Fl::event_y());
            prev_x = Fl::event_x();
            prev_y = Fl::event_y();
            // redraw();
            return 1;
        case FL_DRAG: {
            int t = Fl::event_inside(this);
            if (t) {
                printf("DRAG inside x:%i, y:%i\n", Fl::event_x(), Fl::event_y());
                draw_line(offscreen_buf, Fl::event_x(), Fl::event_y());
                prev_x = Fl::event_x();
                prev_y = Fl::event_y();
                redraw();
            } else {
                printf("DRAG outside x:%i, y:%i\n", Fl::event_x(), Fl::event_y());
                // redraw();
            }
            return 1;
        }
        case FL_RELEASE:
            printf("RELEASE x:%i, y:%i\n", Fl::event_x(), Fl::event_y());
            draw_line(offscreen_buf, Fl::event_x(), Fl::event_y());
            redraw();
            return 1;
        default:
            return Fl_Widget::handle(event);
  }
}
