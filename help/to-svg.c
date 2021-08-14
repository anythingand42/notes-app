#include <cairo.h>
#include <cairo-svg.h> 

int
main (int argc, char *argv[])
{
    cairo_surface_t *surface =
        cairo_image_surface_create (CAIRO_FORMAT_ARGB32, 512, 512);
    cairo_t *cr =
        cairo_create (surface);

    cairo_select_font_face (cr, "serif", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size (cr, 32.0);
    cairo_set_source_rgb (cr, 0.0, 0.0, 1.0);
    cairo_move_to (cr, 10.0, 50.0);
    cairo_show_text (cr, "Hello, world");

    cairo_surface_t * surface_svg = cairo_svg_surface_create("0.svg", 512, 512);
    // cairo_set_source (cr, surface2);
    // cairo_paint(cr);
    cairo_t *cr2 = cairo_create (surface_svg);
    cairo_set_source_surface (cr2, surface, 0, 0);
    cairo_paint (cr2);

    cairo_destroy (cr);
    cairo_destroy (cr2);
    cairo_surface_write_to_png (surface, "hello.png");
    cairo_surface_destroy (surface);
    cairo_surface_destroy (surface_svg);
    return 0;
}
