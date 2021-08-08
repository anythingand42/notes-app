#include <gtk/gtk.h>
#include <cairo-svg.h> 

/* Surface to store current scribbles */
static cairo_surface_t *surface = NULL;
static cairo_t *cr;

static void
clear_surface (void)
{
  cairo_set_source_rgb (cr, 1, 1, 1);
  cairo_paint (cr);
}

/* Create a new surface of the appropriate size to store our scribbles */
static void
resize_cb (GtkWidget *widget,
           int        width,
           int        height,
           gpointer   data)
{
  if (surface)
    {
      cairo_surface_destroy (surface);
      surface = NULL;
    }

  if (gtk_native_get_surface (gtk_widget_get_native (widget)))
    {
      surface = gdk_surface_create_similar_surface (gtk_native_get_surface (gtk_widget_get_native (widget)),
                                                   CAIRO_CONTENT_COLOR,
                                                   gtk_widget_get_width (widget),
                                                   gtk_widget_get_height (widget));
      cr = cairo_create (surface);

      /* Initialize the surface to white */
      clear_surface ();
    }
}

/* Redraw the screen from the surface. Note that the draw
 * callback receives a ready-to-be-used cairo_t that is already
 * clipped to only draw the exposed areas of the widget
 */
static void
draw_cb (GtkDrawingArea *drawing_area,
         cairo_t        *cr,
         int             width,
         int             height,
         gpointer        data)
{
  cairo_set_source_surface (cr, surface, 0, 0);
  cairo_paint (cr);
}

/* Draw a rectangle on the surface at the given position */
// static void
// draw_brush (GtkWidget *widget,
//             double     x,
//             double     y)
// {
//   double w = gtk_widget_get_width (widget);
//   double h = gtk_widget_get_height (widget);

//   // cairo_scale(cr, 0.5, 0.5);
//   // cairo_rectangle (cr, x - 3, y - 3, 6, 6);
//   // cairo_fill (cr);
//   cairo_set_source_rgb (cr, 0, 0, 0);
//   cairo_move_to (cr, 0, 0);
//   cairo_line_to (cr, w, h);
//   cairo_move_to (cr, w, 0);
//   cairo_line_to (cr, 0, h);
//   cairo_set_line_width (cr, 0.2 * w);
//   cairo_stroke (cr);

//   cairo_rectangle (cr, 0, 0, 0.5 * w, 0.5 * h);
//   cairo_set_source_rgba (cr, 1, 0, 0, 0.80);
//   cairo_fill (cr);

//   cairo_rectangle (cr, 0, h * 0.5, w * 0.5, h * 0.5);
//   cairo_set_source_rgba (cr, 0, 1, 0, 0.60);
//   cairo_fill (cr);

//   cairo_rectangle (cr, w * 0.5, 0, w * 0.5, h * 0.5);
//   cairo_set_source_rgba (cr, 0, 0, 1, 0.40);
//   cairo_fill (cr);

//   /* Now invalidate the drawing area. */
//   gtk_widget_queue_draw (widget);
// }

// static double start_x;
// static double start_y;
// static double prev_x;
// static double prev_y;

static void
drag_begin (GtkGestureDrag *gesture,
            double          x,
            double          y,
            GtkWidget      *area)
{
  cairo_select_font_face (cr, "serif", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
  cairo_set_font_size (cr, 32.0);
  cairo_set_source_rgb (cr, 0.0, 0.0, 1.0);
  cairo_move_to (cr, 10.0, 50.0);
  cairo_show_text (cr, "Hello, world");
  gtk_widget_queue_draw (area);
  cairo_surface_t * surface_svg = cairo_svg_surface_create("2.svg", 512, 512);
  cairo_t *cr2 = cairo_create (surface_svg);
  cairo_set_source_surface (cr2, surface, 0, 0);
  cairo_paint (cr2);
  cairo_destroy (cr2);
  cairo_surface_destroy (surface_svg);
}

static void
drag_update (GtkGestureDrag *gesture,
             double          x,
             double          y,
             GtkWidget      *area)
{

}

static void
drag_end (GtkGestureDrag *gesture,
          double          x,
          double          y,
          GtkWidget      *area)
{
  // draw_brush (area, start_x + x, start_y + y);
}

static void
pressed (GtkGestureClick *gesture,
         int              n_press,
         double           x,
         double           y,
         GtkWidget       *area)
{
  clear_surface ();
  gtk_widget_queue_draw (area);
}

static void
close_window (void)
{
  if (surface)
    cairo_surface_destroy (surface);
}

static void
activate (GtkApplication *app,
          gpointer        user_data)
{
  GtkWidget *window;
  GtkWidget *frame;
  GtkWidget *drawing_area;
  GtkGesture *drag;
  GtkGesture *press;

  window = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW (window), "Drawing Area");

  g_signal_connect (window, "destroy", G_CALLBACK (close_window), NULL);

  frame = gtk_frame_new (NULL);
  gtk_window_set_child (GTK_WINDOW (window), frame);

  drawing_area = gtk_drawing_area_new ();
  /* set a minimum size */
  gtk_widget_set_size_request (drawing_area, 512, 512);

  gtk_frame_set_child (GTK_FRAME (frame), drawing_area);

  gtk_drawing_area_set_draw_func (GTK_DRAWING_AREA (drawing_area), draw_cb, NULL, NULL);

  g_signal_connect_after (drawing_area, "resize", G_CALLBACK (resize_cb), NULL);

  drag = gtk_gesture_drag_new ();
  gtk_gesture_single_set_button (GTK_GESTURE_SINGLE (drag), GDK_BUTTON_PRIMARY);
  gtk_widget_add_controller (drawing_area, GTK_EVENT_CONTROLLER (drag));
  g_signal_connect (drag, "drag-begin", G_CALLBACK (drag_begin), drawing_area);
  g_signal_connect (drag, "drag-update", G_CALLBACK (drag_update), drawing_area);
  g_signal_connect (drag, "drag-end", G_CALLBACK (drag_end), drawing_area);

  press = gtk_gesture_click_new ();
  gtk_gesture_single_set_button (GTK_GESTURE_SINGLE (press), GDK_BUTTON_SECONDARY);
  gtk_widget_add_controller (drawing_area, GTK_EVENT_CONTROLLER (press));

  g_signal_connect (press, "pressed", G_CALLBACK (pressed), drawing_area);

  gtk_widget_show (window);
}

int
main (int    argc,
      char **argv)
{
  GtkApplication *app;
  int status;

  app = gtk_application_new ("org.gtk.example", G_APPLICATION_FLAGS_NONE);
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  return status;
}
