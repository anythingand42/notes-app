#include <cairo.h>
#include <gtk/gtk.h>
#include <math.h>
#include <stdlib.h> 

cairo_surface_t *image;
cairo_t *cr;
gdouble rotation = 0;
GtkWidget *window;

gint image_w, image_h;
double DegreesToRadians( int degrees );


double DegreesToRadians( int degrees )
{
    return((double)((double)degrees * ( M_PI/180 )));
}


gboolean rotate_cb( void *degrees )
{
    // Any rotation applied to cr here will be lost, as we create
    // a new cairo context on every expose event
    //cairo_rotate (cr, 4);
    rotation += DegreesToRadians((*(int*)(degrees)));
    //cairo_paint(cr);
    //      printf("rotating\n");
    // Tell our window that it should repaint itself (ie. emit an expose event)
    gtk_widget_queue_draw(window);

    return( TRUE );
}

static gboolean on_expose_event(GtkWidget *widget, GdkEventExpose *event,gpointer data)
{
    // Make sure our window wasn't destroyed yet
    // (to silence a warning)
    g_return_if_fail(GTK_IS_WIDGET(widget));

    cr = gdk_cairo_create (widget->window);
    // We need to apply transformation before setting the source surface
    // We translate (0, 0) to the center of the screen,
    // so we can rotate the image around its center point,
    // not its upper left corner
    cairo_translate(cr, image_w/2, image_h/2);
    cairo_rotate(cr, rotation);
    cairo_set_source_surface(cr, image, -image_w/2, -image_h/2);
    // We need to clip around the image, or cairo will paint garbage data
    //cairo_rectangle(cr, -image_w/2, -image_h/2, image_w, image_h);
    //cairo_clip(cr);

    cairo_paint(cr);
    //printf("Paint\n");
    cairo_destroy(cr);
    return FALSE;
}


int main(int argc, char *argv[])
{   
    int degrees = 10, speed = 125;
    image = cairo_image_surface_create_from_png("wheel.png");
    image_w = cairo_image_surface_get_width(image);
    image_h = cairo_image_surface_get_height(image);

    gtk_init(&argc, &argv);

    if( argc == 3 )
    {
        degrees = atoi(argv[1]);
        speed = atoi(argv[2]);
    }


    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    g_signal_connect(window, "expose-event",
    G_CALLBACK (on_expose_event), NULL);
    g_signal_connect(window, "destroy", G_CALLBACK (gtk_main_quit), NULL);

    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(window), image_w, image_h);
    gtk_widget_set_app_paintable(window, TRUE);

    gtk_widget_show_all(window);
    g_timeout_add(speed, (GSourceFunc) rotate_cb, (void *)&degrees);

    gtk_main();
    cairo_surface_destroy(image);

    return 0;
}