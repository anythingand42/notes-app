#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Menu_Item.H>
#include <FL/Fl_Double_Window.H>
#include <FL/fl_draw.H>
#include <Fl/Fl_Native_File_Chooser.H>
#include <stdio.h>
#include "CanvasBox.hpp"

CanvasBox *canvas;

void open_cb(Fl_Widget*, void*)
{
    Fl_Native_File_Chooser fnfc;
    long len;
    uchar* buf;
    fnfc.title("Open file");
    fnfc.type(Fl_Native_File_Chooser::BROWSE_FILE);
    if ( fnfc.show() ) return;
    FILE* f = fopen(fnfc.filename(), "rb");
    if (!f) {
        perror(fnfc.filename());
        return;
    }
    fseek(f, 0, SEEK_END);
    len = ftell(f);
    fseek(f, 0, SEEK_SET);
    buf = (uchar *)malloc(len);
    if (buf) {
        fread(buf, 1, len, f);
    }
    fclose(f);
    canvas->DrawRGBImage(buf);
}

void save_as_cb() {
    Fl_Native_File_Chooser fnfc;
    fnfc.title("Save File As?");
    fnfc.type(Fl_Native_File_Chooser::BROWSE_SAVE_FILE);
    if (fnfc.show()) return;
    FILE* f = fopen(fnfc.filename(), "wb");
    if (!f) {
        perror(fnfc.filename());
        return;
    }
    fwrite(canvas->GetRGBData(), canvas->GetRGBDataSize(), 1, f);
    fclose(f);
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
    Fl_Window* window = new_view();

    window->show(1, argv);
    return Fl::run();
}
