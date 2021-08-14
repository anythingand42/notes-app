
#include <cairo-svg.h>
#include <cairo-ps.h>
#include <cairo-pdf.h>

#include <FL/fl_draw.H>

#include "CairoBox.h"

CairoBox::CairoBox(int x, int y, int w, int h) : Fl_Box(x,y,w,h)
{
    surface  = NULL; 
    cr       = NULL;
}

cairo_surface_t*  CairoBox::set_surface(int wo, int ho)
{
#ifdef WIN32
#warning win32 mode
    /* Get a Cairo surface for the current DC */
    HDC dc = fl_gc;                                     /* Exported by fltk */
    return cairo_win32_surface_create(dc);
#elif defined (__APPLE__)
#warning Apple Quartz mode
    /* Get a Cairo surface for the current CG context */
    CGContext *ctx = fl_gc;
    return cairo_quartz_surface_create_for_cg_context(ctx, wo, ho);
#else
    /* Get a Cairo surface for the current display */
    return cairo_xlib_surface_create(fl_display, fl_window, fl_visual->visual, wo, ho);
#endif
}

void CairoBox::draw(void) 
{
    // using fltk functions, set up white background with thin black frame
    fl_push_no_clip();            /* remove any clipping region set by the expose events... */
    fl_push_clip(x(), y(), w(), h());
    fl_color(FL_WHITE);
    fl_rectf(x(), y(), w(), h());
    fl_color(FL_BLACK);
    fl_rect(x(), y(), w(), h());

    // set up cairo structures
    surface = set_surface(w(), h());
    cr      = cairo_create(surface);
    /* All Cairo co-ordinates are shifted by 0.5 pixels to correct anti-aliasing */
    // cairo_translate(cr, 0.5, 0.5);
    cairo_set_source_rgb(cr, 0.0, 0.0, 0.0); // set drawing color to black
    cairo_new_path(cr);

    // virtual function defined in driver program
    graphic(cr, x(), y(), w(), h());

    // release the cairo context
    cairo_destroy(cr);
    cairo_surface_destroy(surface);
    //  cr = NULL;

    // remove clip regions
    fl_pop_clip();                          // local clip region
    fl_pop_clip();                          // "no_clip" region
}


