#include <gtk/gtk.h>
#include "base64.h"

static void destroy(GtkWidget *widget, gpointer data)
{
    gtk_main_quit();
}

int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);
    
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Window");
    g_signal_connect(window, "destroy", G_CALLBACK(destroy), NULL);
    
    // gtk_window_fullscreen(GTK_WINDOW(window));

    GtkWidget *image = gtk_image_new_from_file("/home/jsh/overwatch.jpeg");
    // GIcon gicon ;
    // gtk_image_get_gicon(image,)
    gtk_window_resize(GTK_WINDOW(window),1440,2560);

    gtk_container_add(GTK_CONTAINER(window), image);
    gtk_widget_show_all(window);    
    
    gtk_main();
    
    return 0;
}