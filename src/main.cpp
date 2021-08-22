#include <math.h>
#include <stdlib.h>

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Menu_Item.H>
#include <FL/Fl_Double_Window.H>
#include <FL/fl_draw.H>

#include "CanvasBox.hpp"

void new_cb(Fl_Widget*, void* v)
{
  Fl_Window* e = (Fl_Window*)v;
  e->redraw();
}

Fl_Menu_Item menuitems[] = {
    { "&File",              0, 0, 0, FL_SUBMENU  },
        { "&New File",        0, (Fl_Callback *)new_cb },
        { 0 },
    { 0 }
};

Fl_Window* new_view()
{
    Fl_Double_Window* w = new Fl_Double_Window(800, 400, "notes app");

    w->begin();
        Fl_Menu_Bar* m = new Fl_Menu_Bar(0, 0, 800, 30);
        m->copy(menuitems, w);
        CanvasBox *canvas = new CanvasBox(0, 30, 800, 370);
    w->end();

    // canvas->box(FL_FLAT_BOX);
    w->size_range(300,200);
    w->resizable(canvas);

    return w;
}

int main(int argc, char **argv)
{
    Fl_Window* window = new_view();

    window->show(1, argv);
    return Fl::run();
}
