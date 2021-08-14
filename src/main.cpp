#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Menu_Item.H>
#include <FL/Fl_Double_Window.H>
#include <math.h>

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

int CairoBox::handle(int event) {
  switch(event) {
    case FL_PUSH:
        printf("PUSH x:%i, y:%i\n", Fl::event_x(), Fl::event_y());
        // redraw();
        return 1;
    case FL_DRAG: {
        int t = Fl::event_inside(this);
        if (t) {
            printf("DRAG inside x:%i, y:%i\n", Fl::event_x(), Fl::event_y());
            if (Fl::event_x() > 300) {
                ha += 0.05;
            } else {
                ha -= 0.05; 
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
        // redraw();
        return 1;
    default:
      return Fl_Widget::handle(event);
  }
}


void CairoBox::graphic(cairo_t* cr, double x, double y, double w, double h)  
{
    cairo_translate(cr, 0, 30.0);
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
    // cairo_move_to(cr, w / 2, (h - 30.0) / 2);
    cairo_arc (cr, w / 2, (h - 30.0) / 2, 50.0 * ha, 0, 2*M_PI);
    cairo_fill(cr);
    // cairo_line_to(cr, 100.0 * ha, 0.0);
    // cairo_line_to(cr, 100.0 * ha, 100.0 * ha);
    // cairo_line_to(cr, 0.0, 100.0 * ha);
    // cairo_line_to(cr, 0.0, 0.0);
    // cairo_stroke(cr);

    cairo_restore(cr);
}

int main(int argc, char **argv) {
  Fl_Window* window = new_view();

  window->show(1, argv);
  return Fl::run();
}