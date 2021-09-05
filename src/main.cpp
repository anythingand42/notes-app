#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Menu_Item.H>
#include <FL/Fl_Double_Window.H>

#include "CanvasBox.hpp"
#include "FileManager.hpp"

CanvasBox *canvas;
FileManager *file_manager;

void open_cb(Fl_Widget*, void*)
{
    uchar* buf = file_manager->OpenNote();
    if (buf) {
        canvas->DrawRGBImage(buf);
    }
    delete[] buf;
}

void save_as_cb() {
    file_manager->SaveNote(
        canvas->GetRGBData(),
        canvas->GetRGBDataSize()
    );
}

Fl_Menu_Item menuitems[] = {
    { "File",              0, 0, 0, FL_SUBMENU  },
        { "Open File...",        0, (Fl_Callback *)open_cb },
        { "Save as...", FL_COMMAND + FL_SHIFT + 's', (Fl_Callback *)save_as_cb },
        { 0 },
    { 0 }
};

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

int main(int argc, char **argv)
{
    file_manager = new FileManager();
    Fl_Window* window = new_view();

    window->show(1, argv);
    return Fl::run();
}
