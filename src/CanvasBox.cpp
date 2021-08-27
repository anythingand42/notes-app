#include <FL/Enumerations.H>
#include <stdio.h>

#include <FL/Fl.H>
#include <FL/fl_draw.H>

#include "CanvasBox.hpp"

#define BUF_PX_SIZE 2000

int path_prev_x, path_prev_y;
void draw_line(Fl_Offscreen offscreen_buf)
{
    {
        fl_begin_offscreen(offscreen_buf);
        fl_color(FL_BLACK);
        fl_line_style(FL_SOLID | FL_CAP_ROUND, 3);
        fl_line(path_prev_x, path_prev_y, Fl::event_x(), Fl::event_y());
        fl_end_offscreen();
    }
}

int start_text_x, start_text_y;
int text_x, text_y;
int enter_count = 0;
void draw_text(Fl_Offscreen offscreen_buf)
{
    int dx, dy, w, h, del, is_text;
    is_text = Fl::compose(del);
    if (is_text) {
        {
            fl_begin_offscreen(offscreen_buf);
            fl_font(FL_COURIER, 20);
            fl_text_extents(Fl::event_text(), dx, dy, w, h);
            printf("dx: %i, w: %i\n", dx, w);
            fl_color(FL_BLACK);
            fl_draw(Fl::event_text(), text_x, text_y);
            fl_end_offscreen();
        }
        text_x += 13;
    }
    if (Fl::event_key() == FL_BackSpace) {
        printf("backspace\n");
        {
            fl_begin_offscreen(offscreen_buf);
            fl_color(FL_WHITE);
            fl_rectf(text_x - 13, text_y - 16, 13, 20);
            fl_end_offscreen();
        }
        text_x -= 13;
    }
    if (Fl::event_key() == FL_Enter) {
        printf("enter\n");
        text_x = start_text_x;
        start_text_y += 20;
        text_y = start_text_y;
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
            if (Fl::event_button() == FL_LEFT_MOUSE) {
                path_prev_x = Fl::event_x();
                path_prev_y = Fl::event_y();
            } else if (Fl::event_button() == FL_RIGHT_MOUSE) {
                {
                    fl_begin_offscreen(offscreen_buf);
                    fl_color(FL_WHITE);
                    fl_rectf(Fl::event_x() - 15, Fl::event_y() - 15, 30, 30);
                    fl_end_offscreen();
                }
            }
            // redraw();
            return 1;
        case FL_DRAG: {
            int t = Fl::event_inside(this);
            if (t) {
                printf("DRAG inside x:%i, y:%i\n", Fl::event_x(), Fl::event_y());
                if (Fl::event_button() == FL_LEFT_MOUSE) {
                    draw_line(offscreen_buf);
                    path_prev_x = Fl::event_x();
                    path_prev_y = Fl::event_y();
                } else if (Fl::event_button() == FL_RIGHT_MOUSE) {
                    {
                        fl_begin_offscreen(offscreen_buf);
                        fl_color(FL_WHITE);
                        fl_rectf(Fl::event_x() - 15, Fl::event_y() - 15, 30, 30);
                        fl_end_offscreen();
                    }
                }
                redraw();
            } else {
                printf("DRAG outside x:%i, y:%i\n", Fl::event_x(), Fl::event_y());
                // redraw();
            }
            return 1;
        }
        case FL_RELEASE:
            printf("RELEASE x:%i, y:%i\n", Fl::event_x(), Fl::event_y());
            if (Fl::event_button() == FL_LEFT_MOUSE) {
                draw_line(offscreen_buf);
                redraw();
            }
            return 1;
        case FL_ENTER:
            return 1;
        case FL_MOVE:
            // printf("MOVE x:%i, y:%i\n", Fl::event_x(), Fl::event_y());
            start_text_x = Fl::event_x();
            start_text_y = Fl::event_y();
            text_x = Fl::event_x();
            text_y = Fl::event_y();
            return 1;
        case FL_SHORTCUT:
            printf(
                "FL_SHORTCUT event_text: %s, event_key: %d, event_x: %d, event_y: %d\n",
                Fl::event_text(), Fl::event_key(), Fl::event_x(), Fl::event_y()
            );
            draw_text(offscreen_buf);
            redraw();
            return 1;
        default:
            return Fl_Widget::handle(event);
  }
}
