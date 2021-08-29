#include <FL/Enumerations.H>
#include <stdio.h>

#include <FL/Fl.H>
#include <FL/fl_draw.H>

#include "CanvasBox.hpp"

#define BUF_SIZE            2000
#define FONT_HEIGHT         20
#define FONT_WIDTH          13
#define PATH_SIZE           3
#define TEXT_LINE_HEIGHT    22
#define ERASER_SIZE         30

void CanvasBox::PathResetPosition(int x, int y)
{
    path_last_x = x;
    path_last_y = y;
}

void CanvasBox::PathDrawLine()
{
    {
        fl_begin_offscreen(offscreen_buf);
        fl_color(FL_BLACK);
        fl_line_style(FL_SOLID | FL_CAP_ROUND, PATH_SIZE);
        fl_line(path_last_x, path_last_y, Fl::event_x(), Fl::event_y());
        fl_end_offscreen();
    }
}


void CanvasBox::TextResetPosition(int x, int y)
{
    text_start_x = text_x = x;
    text_start_y = text_y = y;
}

void CanvasBox::TextHandleDrawing()
{
    int dx, dy, w, h, del, is_text;
    is_text = Fl::compose(del);
    if (is_text) {
        {
            fl_begin_offscreen(offscreen_buf);
            fl_font(FL_COURIER, FONT_HEIGHT);
            fl_text_extents(Fl::event_text(), dx, dy, w, h);
            // printf("dx: %i, w: %i\n", dx, w);
            fl_color(FL_BLACK);
            fl_draw(Fl::event_text(), text_x, text_y);
            fl_end_offscreen();
        }
        text_x += FONT_WIDTH;
    }
    if (Fl::event_key() == FL_BackSpace) {
        {
            fl_begin_offscreen(offscreen_buf);
            fl_color(FL_WHITE);
            fl_rectf(
                text_x - FONT_WIDTH,
                text_y - FONT_HEIGHT + (FONT_HEIGHT / 5),   // to erase letters like "y"
                FONT_WIDTH, FONT_HEIGHT
            );
            fl_end_offscreen();
        }
        text_x -= FONT_WIDTH;
    }
    if (Fl::event_key() == FL_Enter) {
        text_x = text_start_x;
        text_start_y += TEXT_LINE_HEIGHT;
        text_y = text_start_y;
    }
}

void CanvasBox::EraserDraw(int x, int y)
{
    {
        fl_begin_offscreen(offscreen_buf);
        fl_color(FL_WHITE);
        fl_rectf(
            x - ERASER_SIZE / 2,
            y - ERASER_SIZE / 2,
            ERASER_SIZE,
            ERASER_SIZE
        );
        fl_end_offscreen();
    }
}


void CanvasBox::draw(void)
{
    if (!offscreen_buf) {
        offscreen_buf = fl_create_offscreen(BUF_SIZE, BUF_SIZE);
        {
            fl_begin_offscreen(offscreen_buf);
            fl_color(FL_WHITE);
            fl_rectf(0, 0, BUF_SIZE, BUF_SIZE);
            fl_end_offscreen();
        }
    }
    fl_copy_offscreen(x(), y(), w(), h(), offscreen_buf, x(), y());
}

int CanvasBox::handle(int event)
{
    switch(event) {
        case FL_PUSH:
            // printf("PUSH x:%i, y:%i\n", Fl::event_x(), Fl::event_y());
            if (Fl::event_button() == FL_LEFT_MOUSE) {
                PathResetPosition(Fl::event_x(), Fl::event_y());
            } else if (Fl::event_button() == FL_RIGHT_MOUSE) {
                EraserDraw(Fl::event_x(), Fl::event_y());
                redraw();
            }
            return 1;
        case FL_DRAG: {
            int t = Fl::event_inside(this);
            if (t) {
                // printf("DRAG inside x:%i, y:%i\n", Fl::event_x(), Fl::event_y());
                if (Fl::event_button() == FL_LEFT_MOUSE) {
                    PathDrawLine();
                    PathResetPosition(Fl::event_x(), Fl::event_y());
                } else if (Fl::event_button() == FL_RIGHT_MOUSE) {
                    EraserDraw(Fl::event_x(), Fl::event_y());
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
                PathDrawLine();
                redraw();
            }
            return 1;
        // in order to catch FL_MOVE event
        case FL_ENTER:
            return 1;
        case FL_MOVE:
            TextResetPosition(Fl::event_x(), Fl::event_y());
            return 1;
        case FL_SHORTCUT:
            TextHandleDrawing();
            redraw();
            return 1;
        default:
            return Fl_Widget::handle(event);
    }
}
