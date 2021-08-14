#include <math.h>
#include <stdlib.h>

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Menu_Item.H>
#include <FL/Fl_Double_Window.H>
#include <FL/fl_draw.H>

#include "CanvasBox.h"

struct notes_item {
    double x;
    double y;
    struct notes_item *next;
};

void notes_push(struct notes_item **phead, double x, double y)
{
    struct notes_item *tmp;
    tmp = (notes_item *)malloc(sizeof(struct notes_item));
    tmp->x = x;
    tmp->y = y;
    tmp->next = *phead;
    *phead = tmp;
}

struct notes_item * notes = NULL;
struct notes_item * cur_note = NULL;

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

CanvasBox *canvas  = (CanvasBox *)0;

Fl_Window* new_view()
{
    Fl_Double_Window* w = new Fl_Double_Window(800, 400, "notes app");

    w->begin();
        Fl_Menu_Bar* m = new Fl_Menu_Bar(0, 0, 800, 30);
        m->copy(menuitems, w);
        canvas = new CanvasBox(0, 30, 800, 370);
    w->end();

    // canvas->box(FL_FLAT_BOX);
    w->size_range(300,200);
    w->resizable(canvas);

    return w;
}

double ha = 1.0;

int CanvasBox::handle(int event)
{
  switch(event) {
    case FL_PUSH:
        printf("PUSH x:%i, y:%i\n", Fl::event_x(), Fl::event_y());
        notes_push(&notes, Fl::event_x(), Fl::event_y());
        cur_note = notes;
        redraw();
        return 1;
    case FL_DRAG: {
        int t = Fl::event_inside(this);
        if (t) {
            printf("DRAG inside x:%i, y:%i\n", Fl::event_x(), Fl::event_y());
            notes_push(&notes, Fl::event_x(), Fl::event_y());
            cur_note = notes;
            // if (Fl::event_x() > 300) {
            //     ha += 0.05;
            // } else {
            //     ha -= 0.05; 
            // }
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

void CanvasBox::graphic(double x, double y, double w, double h)  
{
    fl_line_style(FL_SOLID, 4);
    if(cur_note && cur_note->next) {
        printf("print line\n");
        fl_line(cur_note->x, cur_note->y, cur_note->next->x, cur_note->next->y);
    }
    // while(cur_note) {
    //     if (cur_note->next) {
    //         printf("print line\n");
    //         fl_line(cur_note->x, cur_note->y, cur_note->next->x, cur_note->next->y);
    //     }
    //     cur_note = cur_note->next;
    // }
    // fl_line(x, y, w + x, h + y);
}

int main(int argc, char **argv)
{
  Fl_Window* window = new_view();

  window->show(1, argv);
  return Fl::run();
}
