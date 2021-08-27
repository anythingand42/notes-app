#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Check_Button.H>
#include <iostream>
#include <sstream>
#include <string>
#include <list>
#include <vector>

const int hsep = 10, vsep = 10, btnwid = 100, btnhgt = 20;

// The canvas layer
class Canvas : public Fl_Box
{
    Fl_Offscreen& m_buffer;
public:
    Canvas(int x, int y, int w, int h, Fl_Offscreen& buffer)
        : Fl_Box(x, y, w, h)
        , m_buffer(buffer)
    {
    }

    void Clear()
    {
        fl_begin_offscreen(m_buffer);
        fl_color(FL_WHITE);
        fl_rectf(0, 0, w(), h());
        fl_end_offscreen();
    }
    void draw()
    {
        fl_copy_offscreen(x(), y(), w(), h(), m_buffer, 0, 0);
    }
};

// This structure exists because there is no callback that takes
// two client parameters.  Also, the tag must exist for the life of
// the dialog.
class LayerWin;
class LayerCheck
{
public:
    int m_layer;
    std::string m_tag;
    LayerWin* m_parent;
};

// The layer window
class LayerWin
{
    int m_layers;
    Fl_Window* m_dlg;
    std::string m_title;
    std::list<LayerCheck> m_tag;
    Canvas* m_canvas;
    Fl_Offscreen m_buffer;
    std::vector<bool> m_layer;
public:
    LayerWin(int layers)
        : m_layers(layers)
        , m_dlg(0)
    {
    }
    ~LayerWin()
    {
        delete m_dlg;
    }

    void Create()
    {
        int dlgwid = 400, dlghgt = 300;
        std::ostringstream oss;

        oss << m_layers << " Layers";
        m_title = oss.str();
        m_dlg = new Fl_Window(dlgwid, dlghgt, m_title.c_str());
        m_dlg->set_modal();
        int x = hsep, y = vsep;
        for (int ll = 0; ll < m_layers; ++ll)
        {
            oss.str("");
            oss << "Layer " << ll + 1;
            LayerCheck dummy;
            m_tag.push_back(dummy);
            LayerCheck& actual = m_tag.back();
            actual.m_tag = oss.str();
            actual.m_layer = ll;
            actual.m_parent = this;
            Fl_Check_Button* btnCheck = new Fl_Check_Button(x, y, btnwid, btnhgt, actual.m_tag.c_str());
            btnCheck->callback(_LayerCB, &actual);
            m_layer.push_back(false);
            y += vsep + btnhgt;
        }

        x = hsep + btnwid + hsep;
        y = vsep;
        int canw = dlgwid - x - 2 * hsep;
        int canh = dlghgt - 3 * vsep;
        m_buffer = fl_create_offscreen(canw, canh);
        m_canvas = new Canvas(x, y, canw, canh, m_buffer);
        m_canvas->Clear();

        Fl_Button* btnClose = new Fl_Button(hsep, dlghgt - vsep - btnhgt, btnwid, btnhgt, "Close");
        btnClose->callback(_CloseCB, this);
        m_dlg->end();
        m_dlg->show();

        Fl::wait();
    }

    static void _CloseCB(Fl_Widget* w, void* client)
    {
        LayerWin* self = (LayerWin*)client;
        delete self;
    }
    static void _LayerCB(Fl_Widget* w, void* client)
    {
        Fl_Check_Button* btn = (Fl_Check_Button*)w;
        LayerCheck* check = (LayerCheck*)client;
        check->m_parent->LayerCB(check->m_layer, !!btn->value());
    }
    void LayerCB(int layer, bool set)
    {
        std::cout << "Layer " << layer+1 << "=" << set << std::endl;
        m_layer[layer] = set;

        // Redo the canvas
        m_canvas->Clear();
        fl_begin_offscreen(m_buffer);
        for (int ll = 0; ll < m_layer.size(); ++ll)
        {
            if (m_layer[ll])
            {
                switch (ll)
                {
                case 0:
                    fl_color(FL_RED);
                    fl_circle(20, 40, 100);
                    break;
                case 1:
                    fl_color(FL_DARK_GREEN);
                    fl_rectf(50, 70, 20, 40);
                    break;
                case 2:
                    fl_color(FL_BLUE);
                    fl_circle(100, 100, 50);
                    break;
                case 3:
                    fl_color(FL_DARK_MAGENTA);
                    fl_circle(200, 200, 70);
                    break;
                case 4:
                    fl_color(FL_DARK_CYAN);
                    fl_rectf(100, 200, 60, 90);
                    break;
                default:
                    std::cout << "Don't know what to do for layer " << ll + 1;
                    break;
                }
            }
        }
        fl_end_offscreen();
        m_canvas->redraw();
    }
};

void LayerCB(Fl_Widget* w, long layers)
{
    // Create a modal dialog with layers checkbuttons
    LayerWin* dlg = new LayerWin(layers);
    dlg->Create();
}

int main()
{
    // Work out the sizes
    int scrwid = btnwid + 2 * hsep, scrhgt = 2 * vsep + 3 * (vsep + btnhgt);

    // Create the main dialog
    Fl_Window mainwin(scrwid, scrhgt, "Layers");
    int x = hsep;
    int y = vsep;
    Fl_Button* button = new Fl_Button(x, y, btnwid, btnhgt, "3 layers");
    button->callback(LayerCB, 3);

    y += vsep + btnhgt;
    button = new Fl_Button(x, y, btnwid, btnhgt, "4 layers");
    button->callback(LayerCB, 4);

    y += vsep + btnhgt;
    button = new Fl_Button(x, y, btnwid, btnhgt, "5 layers");
    button->callback(LayerCB, 5);
    mainwin.end();
    mainwin.show();
    return Fl::run();
}