#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Menu_Item.H>
#include <FL/Fl_Double_Window.H>

#include "CairoBox.h"

void new_cb(Fl_Widget*, void* v) {
  Fl_Window* e = (Fl_Window*)v;
  e->redraw();
}

Fl_Menu_Item menuitems[] = {
    { "&File",              0, 0, 0, FL_SUBMENU  },
        { "&New File",        0, (Fl_Callback *)new_cb },
        { 0 },
    { 0 }
};

CairoBox *canvas  = (CairoBox *)0;

Fl_Window* new_view() {
    Fl_Double_Window* w = new Fl_Double_Window(800, 400, "notes app");

    w->begin();
        Fl_Menu_Bar* m = new Fl_Menu_Bar(0, 0, 800, 30);
        m->copy(menuitems, w);
        canvas = new CairoBox(0, 30, 800, 370);
    w->end();

    // canvas->box(FL_FLAT_BOX);
    w->size_range(300,200);
    w->resizable(canvas);

    return w;
}

double ha = 1.0;

void CairoBox::graphic(cairo_t* cr, double x, double y, double w, double h)  
{
    cairo_translate(cr, 0, 30);
    cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
    cairo_save(cr);

    // for (double i = 0; i < 500.0; i += 1.0) {
    //     cairo_move_to(cr, 0.0, 0.0);
    //     cairo_line_to(cr, 100.0 * ha + i * 5.0, 0.0);
    //     cairo_line_to(cr, 100.0 * ha + i * 5.0, 100.0 * ha + i * 5.0);
    //     cairo_line_to(cr, 0.0, 100.0 * ha + i * 5.0);
    //     cairo_line_to(cr, 0.0, 0.0);
    //     cairo_stroke(cr);
    // }
    cairo_move_to(cr, 0.0, 0.0);
    cairo_line_to(cr, 100.0 * ha, 0.0);
    cairo_line_to(cr, 100.0 * ha, 100.0 * ha);
    cairo_line_to(cr, 0.0, 100.0 * ha);
    cairo_line_to(cr, 0.0, 0.0);
    cairo_stroke(cr);

    cairo_restore(cr);
}

int main(int argc, char **argv) {
  Fl_Window* window = new_view();

  window->show(1, argv);
  return Fl::run();
}