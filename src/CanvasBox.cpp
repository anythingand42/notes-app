#include <stdio.h>

#include <FL/Fl.H>
#include <FL/fl_draw.H>

#include "CanvasBox.h"

void CanvasBox::draw(void) {
    fl_push_no_clip();                      // remove any clipping region set by the expose events...
    fl_push_clip(x(), y(), w(), h());
    fl_color(FL_WHITE);
    fl_rectf(x(), y(), w(), h());
    fl_color(FL_BLACK);
    fl_rect(x(), y(), w(), h());
    fl_line_style(FL_SOLID, 4);

    graphic(x(), y(), w(), h());

    // remove clip regions
    fl_pop_clip();                          // local clip region
    fl_pop_clip();                          // "no_clip" region
}
