#define BUF_SIZE            2000
#define FONT_HEIGHT         20
#define FONT_WIDTH          13
#define PATH_SIZE           3
#define TEXT_LINE_HEIGHT    22
#define ERASER_SIZE         30

#include <FL/Fl.H>
#include <FL/Fl_Image.H>
#include <FL/fl_draw.H>
#include <random>

#include "OffscreenDrawer.hpp"

std::default_random_engine generator;
std::uniform_int_distribution<int> distribution(30, 230);

OffscreenDrawer::OffscreenDrawer()
{
    offscreen_buf = 0;
}

void OffscreenDrawer::InitBuf()
{
    offscreen_buf = fl_create_offscreen(BUF_SIZE, BUF_SIZE);
    fl_begin_offscreen(offscreen_buf);
    fl_color(FL_WHITE);
    fl_rectf(0, 0, BUF_SIZE, BUF_SIZE);
    fl_color(FL_RED);
    // fl_line(0, 0, 50, 50);
    int size = 4;
    // fl_rectf(0, 30, 40, 40);
    for (int i = 0; i < 300; i++) {
        int x = distribution(generator);
        int y = distribution(generator);
        fl_rectf(x-1, y-1, size, size);
    }
    fl_end_offscreen();
}

Fl_Offscreen OffscreenDrawer::GetBuf()
{
    return offscreen_buf;
}

void OffscreenDrawer::HandlePathStart()
{
    path_last_x = Fl::event_x();
    path_last_y = Fl::event_y();
}

void OffscreenDrawer::HandlePathDraw()
{
    fl_begin_offscreen(offscreen_buf);
    fl_color(FL_BLACK);
    fl_line_style(FL_SOLID | FL_CAP_ROUND, PATH_SIZE);
    fl_line(path_last_x, path_last_y, Fl::event_x(), Fl::event_y());
    fl_end_offscreen();
    path_last_x = Fl::event_x();
    path_last_y = Fl::event_y();
}

void OffscreenDrawer::HandlePathEnd()
{
    fl_begin_offscreen(offscreen_buf);
    fl_color(FL_BLACK);
    fl_line_style(FL_SOLID | FL_CAP_ROUND, PATH_SIZE);
    fl_line(path_last_x, path_last_y, Fl::event_x(), Fl::event_y());
    fl_end_offscreen();
}

void OffscreenDrawer::HandleTextReset()
{
    text_start_x = text_x = Fl::event_x();
    text_start_y = text_y = Fl::event_y();
}

void OffscreenDrawer::HandleTextInput()
{
    int dx, dy, w, h, del, is_text;
    is_text = Fl::compose(del);
    if (is_text) {
        {
            fl_begin_offscreen(offscreen_buf);
            fl_font(FL_COURIER, FONT_HEIGHT);
            fl_text_extents(Fl::event_text(), dx, dy, w, h);
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

void OffscreenDrawer::HandleErase()
{
    fl_begin_offscreen(offscreen_buf);
    fl_color(FL_WHITE);
    fl_rectf(
        Fl::event_x() - ERASER_SIZE / 2,
        Fl::event_y() - ERASER_SIZE / 2,
        ERASER_SIZE,
        ERASER_SIZE
    );
    fl_end_offscreen();
}

uchar* OffscreenDrawer::GetRGBData()
{
    fl_begin_offscreen(offscreen_buf);
    uchar* rgb_data = fl_read_image(NULL, 0, 0, BUF_SIZE, BUF_SIZE);
    fl_end_offscreen();
    return rgb_data;
}

int OffscreenDrawer::GetRGBDataSize()
{
    return BUF_SIZE * BUF_SIZE * 3;
}

void OffscreenDrawer::DrawRGBImage(uchar* rgb_data)
{
    fl_begin_offscreen(offscreen_buf);
    Fl_RGB_Image *img = new Fl_RGB_Image(rgb_data, BUF_SIZE, BUF_SIZE);
    img->draw(0, 0, BUF_SIZE, BUF_SIZE);
    delete img;
    fl_end_offscreen();
}